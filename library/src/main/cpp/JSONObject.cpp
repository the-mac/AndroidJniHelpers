//
// Created by Christopher Miller on 1/20/17.
//

#include "JSONObject.h"

JSONObject::JSONObject() : JavaClass() {}

/**
* Here you can construct the JSONObject object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
JSONObject::JSONObject(JNIEnv *env, std::string json) : JavaClass(env)
{
    initialize(env);

    jstring jsonContent = env->NewStringUTF(json.c_str());
    // WE HAVE ADDED A jstring json PARAMETER TO THE 'JSONObject' constructor
    thisObj = env->NewObject(_clazz, getMethod("<init>"), jsonContent);

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "JSONObject's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

JSONObject::JSONObject(JNIEnv *env, jobject jsonObj) : JavaClass(env)
{
    initialize(env);

    thisObj = jsonObj;

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "JSONObject's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void JSONObject::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    cacheSignature(env, "<init>", "(Ljava/lang/String;)V");
    cacheSignature(env, "has", "(Ljava/lang/String;)Z");
    cacheSignature(env, "getJSONObject", "(Ljava/lang/String;)Lorg/json/JSONObject;");
    cacheSignature(env, "getString", "(Ljava/lang/String;)Ljava/lang/String;");

    registerNativeMethods(env);
}

void JSONObject::mapFields()
{
    //mapField("encryptedString", kTypeString, &encryptedString);
}

jstring JSONObject::getString(JNIEnv *env, std::string stringKey)
{

    jstring key = env->NewStringUTF(stringKey.c_str());
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), key);
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jobject JSONObject::getJSONObject(JNIEnv *env, std::string stringKey)
{

    jstring key = env->NewStringUTF(stringKey.c_str());
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), key);
    JavaExceptionUtils::checkException(env);
    return result;
}

jboolean JSONObject::has(JNIEnv *env, std::string  stringKey)
{
    jstring key = env->NewStringUTF(stringKey.c_str());
    jboolean result = env->CallBooleanMethod(thisObj, getMethod(__FUNCTION__), key);
    JavaExceptionUtils::checkException(env);
    return result;
}
