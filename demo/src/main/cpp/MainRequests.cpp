//
// Created by Christopher Miller on 1/16/17.
//

#include "MainRequests.h"
#include "SharedPreferences.h"
#include <cstring>
#include <AndroidJniApp.h>


MainRequests::MainRequests() : Network() {}

/**
* Here you can construct the MainRequests object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
MainRequests::MainRequests(JNIEnv *env) : Network(env)
{
    initialize(env);

    thisObj = toJavaObject(env);

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "MainRequests's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void MainRequests::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    addNativeSignature("getInstance", (void*)&MainRequests::getInstance, "()Lus/the/mac/library/demo/androidjni/MainRequests;");
    addNativeSignature("getBytes", (void*)&MainRequests::getBytes, "()[B");
    addNativeSignature("getHttpPost", (void*)&MainRequests::getHttpPost, "()Lorg/apache/http/client/methods/HttpPost;");
    addNativeSignature("setRequestType", (void*)&MainRequests::setRequestType, "(I)V");

    registerNativeMethods(env);
}

void MainRequests::mapFields()
{
//    mapField("requestUrl", kTypeString, &requestUrl);
}

jobject MainRequests::getInstance(JNIEnv *env, jobject java_this)
{
    MainRequests *network = new MainRequests(env);
    return network->thisObj;
}


jboolean MainRequests::hasKey(JNIEnv *env) {

    AndroidJniApp context(env, AndroidJniApp::Instance(env));
    SharedPreferences prefs(env, context.thisObj);

    jstring tokenKey = env->NewStringUTF("apiKey");
    return prefs.contains(env, tokenKey);

}

void MainRequests::setKey(JNIEnv *env, jstring token) {

    AndroidJniApp context(env, AndroidJniApp::Instance(env));
    SharedPreferences prefs(env, context.thisObj);

    jstring tokenKey = env->NewStringUTF("apiKey");
    SharedPreferences::Editor editor(env, prefs.edit(env));
    editor.putString(env, tokenKey, token);
    editor.commit(env);

}


jstring MainRequests::getKey(JNIEnv *env) {

    AndroidJniApp context(env, AndroidJniApp::Instance(env));
    SharedPreferences prefs(env, context.thisObj);

    jstring tokenKey = env->NewStringUTF("apiKey");
    return prefs.getString(env, tokenKey, NULL);

}

jbyteArray MainRequests::getBytes(JNIEnv *env, jobject java_this) {
    MainRequests *object = gClasses.getNativeInstance<MainRequests>(env, java_this);

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

jobject MainRequests::getHttpPost(JNIEnv *env, jobject java_this) {
    MainRequests *object = gClasses.getNativeInstance<MainRequests>(env, java_this);

    if (object != NULL)
    {
        if (!object->requestSession.empty()) {
            object->mappingObject["session"] = object->requestSession;
        }

        return Network::getHttpPost(env, java_this);
    }

    return NULL;
}

void MainRequests::setRequestType(JNIEnv *env, jobject java_this, jint requestType) {
    MainRequests *object = gClasses.getNativeInstance<MainRequests>(env, java_this);

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
