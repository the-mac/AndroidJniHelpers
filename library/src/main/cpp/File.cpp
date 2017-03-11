//
// Created by Christopher Miller on 1/12/17.
//

#include "File.h"


void File::initialize(JNIEnv *env)
{
    setClass(env);

    cacheSignature(env, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    cacheSignature(env, "toString", "()Ljava/lang/String;");
    cacheSignature(env, "getAbsolutePath", "()Ljava/lang/String;");
    cacheSignature(env, "exists", "()Z");
    cacheSignature(env, "delete", "()Z");

    registerNativeMethods(env);
}

void File::mapFields()
{
//    mapField("requestUrl", kTypeString, &requestUrl);
}

jstring File::getAbsolutePath(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jboolean File::exists(JNIEnv *env)
{
    jboolean result = env->CallBooleanMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return result;
}

jboolean File::deleteNative(JNIEnv *env)
{
    jboolean result = env->CallBooleanMethod(thisObj, getMethod("delete"));
    JavaExceptionUtils::checkException(env);
    return result;
}