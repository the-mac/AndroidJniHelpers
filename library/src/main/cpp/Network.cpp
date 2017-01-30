//
// Created by Christopher Miller on 1/16/17.
//

#include "Network.h"
#include "HttpPost.h"
#include "ByteArrayEntity.h"
#include <cstring>

static const std::string HEADER_ACCEPT_KEY = "Accept";
static const std::string HEADER_CONTENT_KEY = "Content-type";
static const std::string HEADER_VALUE = "application/json";

Network Instance(JNIEnv * env) {
    if(!networkInstance.isInitialized()) networkInstance.initialize(env);
    return networkInstance;
}

Network::Network() : NativeObject() {}

/**
* Here you can construct the Network object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
Network::Network(JNIEnv *env) : NativeObject(env)
{
    initialize(env);

    thisObj = toJavaObject(env);

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "Network's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void Network::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

//    cacheStaticSignature(env, "testingDefault", "()Lus/the/mac/android/jni/helpers/Network;");
    cacheSignature(env, "getResultString", "()Ljava/lang/String;");
    addNativeMethod("destroy", (void*)&Network::nativeDestroy, kTypeVoid, NULL);
    cacheSignature(env, "setResultString", "(Ljava/lang/String;)V");
    addNativeSignature("getInstance", (void*)&Network::getInstance, "()Lus/the/mac/android/jni/helpers/Network;");
    addNativeSignature("getBytes", (void*)&Network::getBytes, "()[B");
    addNativeSignature("getHttpPost", (void*)&Network::getHttpPost, "()Lorg/apache/http/client/methods/HttpPost;");
    addNativeSignature("put", (void*)&Network::put, "(Ljava/lang/String;Ljava/lang/String;)V");
    addNativeSignature("get", (void*)&Network::get, "(Ljava/lang/String;)Ljava/lang/String;");
    addNativeSignature("toJSONString", (void*)&Network::toJSONString, "()Ljava/lang/String;");
    addNativeSignature("setRequestType", (void*)&Network::setRequestType, "(I)V");
    cacheSignature(env, "request", "(I)Ljava/lang/String;");

    registerNativeMethods(env);
}

void Network::mapFields()
{
//    mapField("requestUrl", kTypeString, &requestUrl);
}

jobject Network::getInstance(JNIEnv *env, jobject java_this)
{
    Network *network = new Network(env);
    return network->thisObj;
}

jstring Network::getResultString(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

void Network::setResultString(JNIEnv *env, jstring jstringValue1)
{
    env->CallVoidMethod(thisObj, getMethod(__FUNCTION__), jstringValue1);
    JavaExceptionUtils::checkException(env);
}

jbyteArray Network::getBytesNative(JNIEnv *env) { return Network::getBytes(env, thisObj); }

jbyteArray Network::getBytes(JNIEnv *env, jobject java_this) {

    const char certificate[] = {

        #include "httpbin_root_certificate" // THIS VALIDATES CONNECTION TO: https://httpbin.org/post
        //FOUND W/: true | openssl s_client -connect httpbin.org:443 2>/dev/null | openssl x509 -in /dev/stdin

        // #include "github_root_certificate" // TRY THIS INSTEAD FOR VALIDATION OF httpbin_root_certificate
        //FOUND W/: true | openssl s_client -connect gist.githubusercontent.com:443 2>/dev/null | openssl x509 -in /dev/stdin

    };

    int size = std::strlen(certificate);

    jbyte *data = (jbyte *) certificate;
    jbyteArray array = env->NewByteArray(size);
    env->SetByteArrayRegion(array, 0, size, data);

    return array;
}


jobject Network::getHttpPostNative(JNIEnv *env) { return Network::getHttpPost(env, thisObj); }

jobject Network::getHttpPost(JNIEnv *env, jobject java_this) {
    Network *object = gClasses.getNativeInstance<Network>(env, java_this);

    if (object != NULL)
    {
        // Setting the url and the post data
        HttpPost httpost(env, object->requestUrl);
        JavaString jsonString(object->requestJSON());
        httpost.setEntity(env, ByteArrayEntity(env, jsonString.toByteArray(env)));

        // Setting a request header so the page receving
        // the request will know what to do with it
        httpost.setHeader(env, HEADER_ACCEPT_KEY, HEADER_VALUE);
        httpost.setHeader(env, HEADER_CONTENT_KEY, HEADER_VALUE);

        return httpost.thisObj;
    }

    return NULL;
}


void Network::putNative(JNIEnv *env, jstring jstringValue1, jstring jstringValue2) { Network::put(env, thisObj, jstringValue1, jstringValue2); }

void Network::put(JNIEnv *env, jobject java_this, jstring key, jstring value) {
    Network *object = gClasses.getNativeInstance<Network>(env, java_this);

    if (object != NULL)
    {
        std::string localKey = env->GetStringUTFChars(key, 0);
        std::string localValue = env->GetStringUTFChars(value, 0);

        object->mappingObject[localKey] = localValue;
    }

}


jstring Network::getNative(JNIEnv *env, jstring jstringValue1) { return Network::get(env, thisObj, jstringValue1); }

jstring Network::get(JNIEnv *env, jobject java_this, jstring key) {
    Network *object = gClasses.getNativeInstance<Network>(env, java_this);

    if (object != NULL)
    {
        std::string localKey = env->GetStringUTFChars(key, 0);
        return env->NewStringUTF(object->mappingObject[localKey].c_str());

    }

    return NULL;
}


std::string Network::requestJSON() {
    stringstream ss;
    auto pair = mappingObject.begin();

    ss << "{ ";

    while (pair != mappingObject.end()) {
        if(pair != mappingObject.begin()) ss << ", ";
        ss << "\"" << pair->first << "\": \"" << pair->second << "\"";
        ++pair;
    }

    ss << " }";

    return ss.str();
}

jstring Network::toJSONStringNative(JNIEnv *env) { return Network::toJSONString(env, thisObj); }

jstring Network::toJSONString(JNIEnv *env, jobject java_this) {
    Network *object = gClasses.getNativeInstance<Network>(env, java_this);

    if (object != NULL)
    {
        std::string contents = object->requestJSON();
        return env->NewStringUTF(contents.c_str());
    }

    return NULL;
}


void Network::setRequestTypeNative(JNIEnv *env, jint jintValue1) { Network::setRequestType(env, thisObj, jintValue1); }

void Network::setRequestType(JNIEnv *env, jobject java_this, jint requestType) {
    Network *object = gClasses.getNativeInstance<Network>(env, java_this);

    if (object != NULL)
    {
        switch (requestType) {
            case HTTP_BIN: object->requestUrl = "https://httpbin.org/post"; break;
            case JSON_TEST: object->requestUrl = "http://ip.jsontest.com"; break;
        }
    }

}


jstring Network::request(JNIEnv *env, jint jintValue1)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), jintValue1);
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

void Network::nativeDestroy(JNIEnv *env, jobject java_this) {
    Network *object = gClasses.getNativeInstance<Network>(env, java_this);
    if (object != NULL) {
        // NativeObject *native_object = dynamic_cast<NativeObject*>(object);
        object->destroy(env, java_this);
    }
}
