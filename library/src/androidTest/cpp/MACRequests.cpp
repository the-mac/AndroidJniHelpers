//
// Created by Christopher Miller on 1/16/17.
//

#include "MACRequests.h"
#include <cstring>


MACRequests::MACRequests() : Network() {}

/**
* Here you can construct the MACRequests object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
MACRequests::MACRequests(JNIEnv *env) : Network(env)
{
    initialize(env);

    thisObj = toJavaObject(env);

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "MACRequests's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void MACRequests::initialize(JNIEnv *env)
{
    cacheConstructor(env);

    addNativeSignature("getInstance", (void*)&MACRequests::getInstance, "()Lus/the/mac/android/jni/helpers/MACRequests;");
    addNativeSignature("getBytes", (void*)&MACRequests::getBytes, "()[B");
    addNativeSignature("getHttpPost", (void*)&MACRequests::getHttpPost, "()Lorg/apache/http/client/methods/HttpPost;");
    addNativeSignature("setRequestType", (void*)&MACRequests::setRequestType, "(I)V");

    registerNativeMethods(env);
}

void MACRequests::mapFields()
{
//    mapField("requestUrl", kTypeString, &requestUrl);
}

jobject MACRequests::getInstance(JNIEnv *env, jobject java_this)
{
    MACRequests *network = new MACRequests(env);
    return network->thisObj;
}

jbyteArray MACRequests::getBytes(JNIEnv *env, jobject java_this) {
    MACRequests *object = gClasses.getNativeInstance<MACRequests>(env, java_this);

    if (object != NULL) {

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
    return NULL;
}

jobject MACRequests::getHttpPost(JNIEnv *env, jobject java_this) {
    MACRequests *object = gClasses.getNativeInstance<MACRequests>(env, java_this);

    if (object != NULL)
    {
        if (!object->requestSession.empty()) {
            object->mappingObject["session"] = object->requestSession;
        }

        return Network::getHttpPost(env, java_this);
    }

    return NULL;
}

void MACRequests::setRequestType(JNIEnv *env, jobject java_this, jint requestType) {
    MACRequests *object = gClasses.getNativeInstance<MACRequests>(env, java_this);

    if (object != NULL) {
        switch (requestType) {
            case HTTP_BIN:
                object->requestUrl = "https://httpbin.org/post";
                object->requestSession = "SESSION-TOKEN-ONE";
                break;
            case JSON_TEST:
                object->requestUrl = "http://ip.jsontest.com";
                object->requestSession = "SESSION-TOKEN-TWO";
                break;
        }
    }
}
