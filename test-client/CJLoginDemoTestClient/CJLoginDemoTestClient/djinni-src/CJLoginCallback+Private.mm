// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#import "CJLoginCallback+Private.h"
#import "CJLoginCallback.h"
#import "DJICppWrapperCache+Private.h"
#import "DJIError.h"
#import "DJIMarshal+Private.h"
#import "DJIObjcWrapperCache+Private.h"
#include <exception>
#include <stdexcept>
#include <utility>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@interface CJLoginCallbackCppProxy : NSObject<CJLoginCallback>

- (id)initWithCpp:(const std::shared_ptr<::cjlogin::LoginCallback>&)cppRef;

@end

@implementation CJLoginCallbackCppProxy {
    ::djinni::CppProxyCache::Handle<std::shared_ptr<::cjlogin::LoginCallback>> _cppRefHandle;
}

- (id)initWithCpp:(const std::shared_ptr<::cjlogin::LoginCallback>&)cppRef
{
    if (self = [super init]) {
        _cppRefHandle.assign(cppRef);
    }
    return self;
}

- (void)complete:(int32_t)errcode
          errmsg:(nonnull NSString *)errmsg
     loginTicket:(nonnull NSString *)loginTicket {
    try {
        _cppRefHandle.get()->complete(::djinni::I32::toCpp(errcode),
                                      ::djinni::String::toCpp(errmsg),
                                      ::djinni::String::toCpp(loginTicket));
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

namespace djinni_generated {

class LoginCallback::ObjcProxy final
: public ::cjlogin::LoginCallback
, private ::djinni::ObjcProxyBase<ObjcType>
{
    friend class ::djinni_generated::LoginCallback;
public:
    using ObjcProxyBase::ObjcProxyBase;
    void complete(int32_t c_errcode, const std::string & c_errmsg, const std::string & c_loginTicket) override
    {
        @autoreleasepool {
            [djinni_private_get_proxied_objc_object() complete:(::djinni::I32::fromCpp(c_errcode))
                                                        errmsg:(::djinni::String::fromCpp(c_errmsg))
                                                   loginTicket:(::djinni::String::fromCpp(c_loginTicket))];
        }
    }
};

}  // namespace djinni_generated

namespace djinni_generated {

auto LoginCallback::toCpp(ObjcType objc) -> CppType
{
    if (!objc) {
        return nullptr;
    }
    if ([(id)objc isKindOfClass:[CJLoginCallbackCppProxy class]]) {
        return ((CJLoginCallbackCppProxy*)objc)->_cppRefHandle.get();
    }
    return ::djinni::get_objc_proxy<ObjcProxy>(objc);
}

auto LoginCallback::fromCppOpt(const CppOptType& cpp) -> ObjcType
{
    if (!cpp) {
        return nil;
    }
    if (auto cppPtr = dynamic_cast<ObjcProxy*>(cpp.get())) {
        return cppPtr->djinni_private_get_proxied_objc_object();
    }
    return ::djinni::get_cpp_proxy<CJLoginCallbackCppProxy>(cpp);
}

}  // namespace djinni_generated

@end
