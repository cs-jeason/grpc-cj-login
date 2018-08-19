// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#import <Foundation/Foundation.h>
@class CJCjLogin;
@protocol CJCheckLoginCallback;
@protocol CJConnectCallback;
@protocol CJLoginCallback;
@protocol CJLogoutCallback;
@protocol CJRegisterUserCallback;


@interface CJCjLogin : NSObject

+ (nullable CJCjLogin *)create:(nonnull NSString *)server
                    serverCert:(nonnull NSString *)serverCert
                          cert:(nonnull NSString *)cert
                           key:(nonnull NSString *)key
                   sslOverride:(nonnull NSString *)sslOverride;

- (void)registerUser:(nonnull NSString *)userName
            password:(nonnull NSString *)password
                  cb:(nullable id<CJRegisterUserCallback>)cb;

- (void)login:(nonnull NSString *)userName
     password:(nonnull NSString *)password
           cb:(nullable id<CJLoginCallback>)cb;

- (void)checkLogin:(nonnull NSString *)userName
       loginTicket:(nonnull NSString *)loginTicket
                cb:(nullable id<CJCheckLoginCallback>)cb;

- (void)logout:(nonnull NSString *)userName
    sessionKey:(nonnull NSString *)sessionKey
            cb:(nullable id<CJLogoutCallback>)cb;

- (void)connect:(nonnull NSString *)userName
     sessionKey:(nonnull NSString *)sessionKey
             cb:(nullable id<CJConnectCallback>)cb;

@end
