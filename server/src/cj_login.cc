#include "proto/cj_login.grpc.pb.h"
#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <thread>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>

#include "cj_login_client.h"
#include "helper.h"
#include "semaphore.hpp"
#include "connected_client.hpp"

using cjLogin::ErrCode;
using cjLogin::CjLoginCGI;

using grpc::InsecureServerCredentials;
using grpc::InsecureChannelCredentials;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;

using cjLogin::ConnectRequest;
using cjLogin::ServerMessage;
using cjLogin::MessageType;

class CjLoginCGIImp final : public CjLoginCGI::Service {
 public:
  CjLoginCGIImp() {
    this->server = new CjLoginClient(CreateChannel("localhost:50051",
                                                   InsecureChannelCredentials()));
    InternalConnectRequest request;

    this->server->internalConnect(request, [this](InternalMessage &message) {
        auto uin = std::to_string(message.uin());
        ServerMessage toClientMessage;
        toClientMessage.set_type(message.type());
        toClientMessage.set_content(message.content());
        if (this->writeMessageToClient(uin, toClientMessage)) {
          if (message.type() == MessageType::LOGOUT_BY_OTHERS ||
              message.type() == MessageType::LOGOUT_BY_SYSTEM) {
            this->deleteClient(uin);
          }
        }
      });
  }

  void deleteClient(const string &uin) {
    std::unique_lock<std::mutex> lock(mtx);
    auto iter = this->clientMap.find(uin);
    if (iter != this->clientMap.end()) {
      auto client = this->clientMap[uin];
      client->s.signal();
      this->clientMap.erase(uin);
    }
  }

  ~CjLoginCGIImp() {
    delete this->server;
  }

  Status connect(ServerContext *context,
                 const ConnectRequest *request,
                 ServerWriter<ServerMessage> *writer) override {

    auto userName = request->basereq().username();
    auto sessionKey = request->basereq().sessionkey();

    PayloadInfo payload;
    if (!cjLogin::extraSessionKey(sessionKey, payload)
        || payload.userName != userName) {
      LOG(ERROR) << "connect request invalid" << userName;
      return Status::CANCELLED;
    }

    auto client = new ConnectedClient<ServerMessage>();
    client->context = context;
    client->writer = writer;

    auto iter = this->clientMap.find(payload.uin);
    if (iter != this->clientMap.end()) {
      this->deleteClient(payload.uin);
    }
    {
      std::unique_lock<std::mutex> lock(mtx);
      this->clientMap[payload.uin] = client;
    }
    client->s.wait();

    delete client;
    return Status::OK;
  }

  Status registerUser(ServerContext *context,
                      const RegisterUserRequest *request,
                      RegisterUserResponse *response) override {
    return this->server->registerUser(*request, response);
  }

  Status login(ServerContext *context,
               const UserLoginRequest *request,
               UserLoginResponse *response) override {
    return this->server->login(*request, response);
  }

  Status checkLogin(ServerContext *context,
                    const UserCheckLoginRequest *request,
                    UserCheckLoginResponse *response) override {
    return this->server->checkLogin(*request, response);
  }

  Status logout(ServerContext *context,
                const UserLogoutRequest *request,
                UserLogoutResponse *response) override {
    auto userName = request->basereq().username();
    auto sessionKey = request->basereq().sessionkey();
    auto baseResponse = response->mutable_baseresp();

    PayloadInfo payload;
    if (!cjLogin::extraSessionKey(sessionKey, payload)
        || payload.userName != userName) {
      LOG(ERROR) << "logout User Invalid request invalid" << userName;
      baseResponse->set_errcode(ErrCode::SYSTEM_INVALID_PARAM);
      baseResponse->set_errmsg("invalid params");
      return Status::OK;
    }

    auto iter = this->clientMap.find(payload.uin);
    if (iter != this->clientMap.end()) {
      this->deleteClient(payload.uin);
    }
    return this->server->logout(*request, response);
  }

 private:
  CjLoginClient *server;
  std::map<std::string, ConnectedClient<ServerMessage> *> clientMap;
  std::mutex mtx;

  bool writeMessageToClient(std::string uin, ServerMessage &message) {

    auto iter = this->clientMap.find(uin);
    if (iter != this->clientMap.end()) {
      auto client = this->clientMap[uin];
      if (!client->writer || !client->context) {
        this->deleteClient(uin);
        LOG(ERROR) << "write message to unconnected client";
        return false;
      }

      if (client->context->IsCancelled()) {
        this->deleteClient(uin);
        LOG(ERROR) << "write message to cancelled client";
        return false;
      }

      client->writer->Write(message);
      std::cout << "write message to client: " << uin
                << ", type: " << message.type()
                << ", content: " << message.content() << std::endl;
      return true;
    } else {
      return false;
    }
  }
};


std::string getFileContent(const char *fpath)
{
  std::ifstream finstream(fpath);
  std::string contents((std::istreambuf_iterator<char>(finstream)),
                       std::istreambuf_iterator<char>());
  return contents;
}

void RunServer(int argc, char **argv) {
  std::string address("0.0.0.0:50052");

  auto clientCA = getFileContent(argv[1]); // for verifying clients
  auto serverKey = getFileContent(argv[2]);
  auto serverCert = getFileContent(argv[3]);

  grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp = {
    serverKey.c_str(), serverCert.c_str()
  };
  grpc::SslServerCredentialsOptions ssl_opts(GRPC_SSL_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_AND_VERIFY);
  ssl_opts.pem_root_certs = clientCA;
  ssl_opts.pem_key_cert_pairs.push_back(pkcp);

  std::cout << "pem_root_certs length: " << ssl_opts.pem_root_certs.length() << std::endl;

  std::shared_ptr<grpc::ServerCredentials> creds = grpc::SslServerCredentials(ssl_opts);

  CjLoginCGIImp service;
  ServerBuilder builder;
  builder.AddListeningPort(address, creds);
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server listening on " << address;
  server->Wait();
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);

  RunServer(argc, argv);
  return 0;
}
