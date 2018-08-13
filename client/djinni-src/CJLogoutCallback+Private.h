// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#include "logoutCallback.hpp"
#include <memory>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@protocol CJLogoutCallback;

namespace djinni_generated {

class LogoutCallback
{
public:
    using CppType = std::shared_ptr<::cjlogin::LogoutCallback>;
    using CppOptType = std::shared_ptr<::cjlogin::LogoutCallback>;
    using ObjcType = id<CJLogoutCallback>;

    using Boxed = LogoutCallback;

    static CppType toCpp(ObjcType objc);
    static ObjcType fromCppOpt(const CppOptType& cpp);
    static ObjcType fromCpp(const CppType& cpp) { return fromCppOpt(cpp); }

private:
    class ObjcProxy;
};

}  // namespace djinni_generated
