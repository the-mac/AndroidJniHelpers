//
// Created by Christopher Miller on 1/16/17.
//

#include "Network.h"
#include <cstring>

Network::Network() : NativeObject() {}

/**
* Here you can construct the Network object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
Network::Network(JNIEnv *env) : NativeObject(env)
{
    initialize(env);

    thisObj = toJavaObject(env); // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod(getCanonicalName()));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "Network's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void Network::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    addJavaSignature("getResultString", "()Ljava/lang/String;");
    addJavaSignature("setResultString", "(Ljava/lang/String;)V");
    addNativeSignature("destroy", (void*)&Network::destroy, "()V");
    addNativeSignature("getBytes", (void*)&Network::getBytes, "()[B");
    addNativeSignature("getHttpPost", (void*)&Network::getHttpPost, "()Ljava/lang/Object;");
    addNativeSignature("put", (void*)&Network::put, "(Ljava/lang/String;Ljava/lang/String;)V");
    addNativeSignature("get", (void*)&Network::get, "(Ljava/lang/String;)Ljava/lang/String;");
    addNativeSignature("toJSONString", (void*)&Network::toJSONString, "()Ljava/lang/String;");
    addNativeSignature("setRequestType", (void*)&Network::setRequestType, "(I)V");
    addJavaSignature("request", "(I)Ljava/lang/String;");

    registerNativeMethods(env);
}

void Network::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}

jobject Network::getInstance(JNIEnv *env, jobject java_this)
{
    Network *network = new Network(env);
    return network->thisObj;
}

jobject Network::testingDefault(JNIEnv *env)
{
    jobject result = env->CallStaticObjectMethod(getClass(env), getStaticMethod(env, getClass(env), __FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return result;
}

jstring Network::getResultString(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getJavaMethod(env, __FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

void Network::setResultString(JNIEnv *env, jstring jstringValue1)
{
    env->CallVoidMethod(thisObj, getJavaMethod(env, __FUNCTION__), jstringValue1);
    JavaExceptionUtils::checkException(env);
}

void Network::destroyNative(JNIEnv *env) { Network::destroy(env, thisObj); }

void Network::destroy(JNIEnv *env, jobject java_this) {
    Network *object = gClasses.getNativeInstance<Network>(env, java_this);

    if (object != NULL)
    {
        // TODO: ADD YOUR NATIVE IMPLMENTATION HERE (i.e. object->callToSomeFunction())
    }

}


jbyteArray Network::getBytesNative(JNIEnv *env) { return Network::getBytes(env, thisObj); }

jbyteArray Network::getBytes(JNIEnv *env, jobject java_this) {

    const char certificate[] = {

        #include "httpbin_root_certificate" // THIS VALIDATES CONNECTION TO: https://httpbin.org/post
        //FOUND W/ "true | openssl s_client -connect httpbin.org:443 2>/dev/null | openssl x509 -in /dev/stdin"

        // #include "github_root_certificate" // TRY THIS INSTEAD FOR VALIDATION OF httpbin_root_certificate
        //FOUND W/ "true | openssl s_client -connect gist.githubusercontent.com:443 2>/dev/null | openssl x509 -in /dev/stdin"

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
//        //url with the post data
//        HttpPost httpost(env, object->requestUrl);
//
//        //gets the json post request as bytes
//        std::string jsonString = object->toJSON();
//        const char *json = jsonString.c_str();
//        jbyte *data = (jbyte *) json;
//
//        int size = std::strlen(json);
//        jbyteArray outputData = env->NewByteArray(size);
//        env->SetByteArrayRegion(outputData, 0, size, data);
//        httpost.setEntity(ByteArrayEntity(outputData));
//
//        //sets a request header so the page receving the request
//        //will know what to do with it
//        httpost.setHeader("Accept", "application/json");
//        httpost.setHeader("Content-type", "application/json");
//
//        return httpost.thisObj;
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


std::string Network::toJSON() {
    stringstream ss;
    auto pair = mappingObject.begin();

    ss << "{ ";

    while (pair != mappingObject.end()) {
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
        std::string contents = object->toJSON();
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
    jobject result = env->CallObjectMethod(thisObj, getJavaMethod(env, __FUNCTION__), jintValue1);
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}