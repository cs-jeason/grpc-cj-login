// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#include "NativeCjLogin.hpp"  // my header

namespace djinni_generated {

NativeCjLogin::NativeCjLogin() : ::djinni::JniInterface<::cjlogin::CjLogin, NativeCjLogin>("com/jeason/cjlogin/CjLogin$CppProxy") {}

NativeCjLogin::~NativeCjLogin() = default;


CJNIEXPORT void JNICALL Java_com_jeason_cjlogin_CjLogin_00024CppProxy_nativeDestroy(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        delete reinterpret_cast<::djinni::CppProxyHandle<::cjlogin::CjLogin>*>(nativeRef);
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

CJNIEXPORT jobject JNICALL Java_com_jeason_cjlogin_CjLogin_00024CppProxy_create(JNIEnv* jniEnv, jobject /*this*/)
{
    try {
        DJINNI_FUNCTION_PROLOGUE0(jniEnv);
        auto r = ::cjlogin::CjLogin::create();
        return ::djinni::release(::djinni_generated::NativeCjLogin::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

}  // namespace djinni_generated