//
// Created by Christopher Miller on 2/17/17.
//

#include "SecretKeySpec.h"

SecretKeySpec::SecretKeySpec() : JavaClass() {}

/**
* Here you can construct the SecretKeySpec object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
SecretKeySpec::SecretKeySpec(JNIEnv *env, jbyteArray array, jint start, jint end) : JavaClass(env)
{
    initialize(env);

    jstring algorithm = JavaString("AES").toJavaString(env);
    thisObj = env->NewObject(_clazz, getMethod("<init>"), array, start, end, algorithm);

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "SecretKeySpec's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void SecretKeySpec::initialize(JNIEnv *env)
{
    setClass(env);

    cacheSignature(env, "<init>", "([BIILjava/lang/String;)V");
    cacheSignature(env, "getAlgorithm", "()Ljava/lang/String;");
    cacheSignature(env, "getFormat", "()Ljava/lang/String;");
    cacheSignature(env, "getEncoded", "()[B");
    cacheSignature(env, "hashCode", "()I");
    cacheSignature(env, "equals", "(Ljava/lang/Object;)Z");

    registerNativeMethods(env);
}

void SecretKeySpec::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}

jstring SecretKeySpec::getAlgorithm(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jstring SecretKeySpec::getFormat(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jbyteArray SecretKeySpec::getEncoded(JNIEnv *env)
{
    jbyteArray result = (jbyteArray) env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return result;
}

jint SecretKeySpec::hashCode(JNIEnv *env)
{
    jint result = env->CallIntMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return result;
}

jboolean SecretKeySpec::equals(JNIEnv *env, jobject jobjectValue1)
{
    jboolean result = env->CallBooleanMethod(thisObj, getMethod(__FUNCTION__), jobjectValue1);
    JavaExceptionUtils::checkException(env);
    return result;
}