//
// Created by Christopher Miller on 2/16/17.
//

#include "Base64.h"

static Base64 *base64Instance = NULL;

/**
* The getStaticMethod method is used to get the Jni Helper's
* static method for the Base64 class defined in Java.
*/
static jmethodID getStaticMethod(JNIEnv *env, const char *method_name) {
//    if(base64Instance = NULL) base64Instance = new Base64(env);
//    return base64Instance->getMethod(method_name);

//    jclass _class = env->FindClass("org/apache/commons/codec/binary/Base64");
//    jmethodID method = env->GetStaticMethodID(_class, __FUNCTION__, "()Lorg/apache/commons/codec/binary/Base64;");
//    jbyteArray result = (jbyteArray) env->CallStaticObjectMethod(_class, method, contents);

//    jclass _class = env->FindClass("org/apache/commons/codec/binary/Base64");
    jmethodID method = env->GetStaticMethodID(Base64::getClass(env), method_name, "()Lorg/apache/commons/codec/binary/Base64;");
    return method;
}

Base64::Base64() : JavaClass() {}

/**
* Here you can construct the Base64 object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
Base64::Base64(JNIEnv *env) : JavaClass(env)
{
    initialize(env);

    thisObj = toJavaObject(env); // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "Base64's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void Base64::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    cacheStaticSignature(env, "decodeBase64", "([BI)[B");
    cacheStaticSignature(env, "encodeBase64", "([BI)[B");

//    cacheSignature(env, "android/util/Base64", "()V");
//    cacheStaticSignature(env, "decode", "(Ljava/lang/String;I)[B");
//    cacheStaticSignature(env, "decode", "([BIII)[B");
//    cacheStaticSignature(env, "encodeToString", "([BI)Ljava/lang/String;");
//    cacheStaticSignature(env, "encodeToString", "([BIII)Ljava/lang/String;");
//    cacheStaticSignature(env, "encode", "([BI)[B");
//    cacheStaticSignature(env, "encode", "([BIII)[B");

    registerNativeMethods(env);
}

void Base64::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}

//jobject Base64::android.util.Base64(JNIEnv *env)
//{
//jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
//JavaExceptionUtils::checkException(env);
//return result;
//}

jbyteArray Base64::decodeBase64(JNIEnv *env, jbyteArray contents)
{
    jclass _class = env->FindClass("org/apache/commons/codec/binary/Base64");
    jmethodID method = env->GetStaticMethodID(_class, __FUNCTION__, "([B)[B");
    jbyteArray result = (jbyteArray) env->CallStaticObjectMethod(_class, method, contents);
    JavaExceptionUtils::checkException(env);
    return result;
}

jbyteArray Base64::encodeBase64(JNIEnv *env, jbyteArray contents)
{
    jclass _class = env->FindClass("org/apache/commons/codec/binary/Base64");
    jmethodID method = env->GetStaticMethodID(_class, __FUNCTION__, "([B)[B");
    jbyteArray result = (jbyteArray) env->CallStaticObjectMethod(_class, method, contents);
    JavaExceptionUtils::checkException(env);
    return result;
}

//jbyteArray Base64::decode(JNIEnv *env, jstring jstringValue1, jint jintValue2)
//{
//    jbyteArray result = (jbyteArray) env->CallStaticObjectMethod(getClass(env), getStaticMethod(__FUNCTION__), jstringValue1, jintValue2);
//    JavaExceptionUtils::checkException(env);
//    return result;
//}
//
//jbyteArray Base64::decode(JNIEnv *env, jstring jstringValue1, jint jintValue2)
//{
//    jbyteArray result = (jbyteArray) env->CallStaticObjectMethod(getClass(env), getStaticMethod(__FUNCTION__), jstringValue1, jintValue2);
//    JavaExceptionUtils::checkException(env);
//    return result;
//}

jstring Base64::encodeToString(JNIEnv *env, jbyteArray jbyteArrayValue1, jint jintValue2)
{
    jobject result = env->CallStaticObjectMethod(getClass(env), getStaticMethod(env, __FUNCTION__), jbyteArrayValue1, jintValue2);
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

//jstring Base64::encodeToString(JNIEnv *env, jbyteArray jbyteArrayValue1, jint jintValue2)
//{
//    jobject result = env->CallStaticObjectMethod(getClass(env), getStaticMethod(__FUNCTION__), jbyteArrayValue1, jintValue2);
//    JavaExceptionUtils::checkException(env);
//    return (jstring) result;
//}

jbyteArray Base64::encode(JNIEnv *env, jbyteArray jbyteArrayValue1, jint jintValue2)
{
    jbyteArray result = (jbyteArray) env->CallStaticObjectMethod(getClass(env), getStaticMethod(env, __FUNCTION__), jbyteArrayValue1, jintValue2);
    JavaExceptionUtils::checkException(env);
    return result;
}

//jbyteArray Base64::encode(JNIEnv *env, jbyteArray jbyteArrayValue1, jint jintValue2)
//{
//    jbyteArray result = (jbyteArray) env->CallStaticObjectMethod(getClass(env), getStaticMethod(__FUNCTION__), jbyteArrayValue1, jintValue2);
//    JavaExceptionUtils::checkException(env);
//    return result;
//}

