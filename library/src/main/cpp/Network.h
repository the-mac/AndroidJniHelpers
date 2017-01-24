//
// Created by Christopher Miller on 1/16/17.
//

#ifndef APPLICATIONRESOURCES_NETWORK_H
#define APPLICATIONRESOURCES_NETWORK_H

#include "JniHelpers.h"
using namespace std;

class Network : public NativeObject {

protected:
    jobject thisObj;
    std::string requestUrl;
    std::map<std::string,std::string> mappingObject;

public:
    JavaString resultString;
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the Network class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", Network);
    }
//    static const char *getCanonicalName(const char *method_name) {
//        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers/Network", method_name);
//    }
//    /**
//    * This facsimile of the Java method java.lang.Object.getClass() is used to maintain
//    * the Jni Helper's static relationship to the Network class defined in Java.
//    */
//    static jclass getClass(JNIEnv *env) {
//        return env->FindClass(MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", Network));
//    }

    Network();

    Network(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

    std::string toJSON();


    /**
    * The getBytes method is used to get the Server's Certificate
    * @return SSL (Secure Sockets Layer) certificate bytes for a secure network connection.
    */
    static jbyteArray getBytes(JNIEnv *env, jobject java_this);

    /**
    * The setRequestType method is used to set the request type
    * Can set any variables needed for a specific session (url, session key, etc.)
    */
    static void setRequestType(JNIEnv *env, jobject java_this, jint jintValue1);


    jbyteArray getBytesNative(JNIEnv *env);

    void setRequestTypeNative(JNIEnv *env, jint jintValue1);


    static jobject getInstance(JNIEnv *env, jobject java_this);

    static jobject testingDefault(JNIEnv *env);

    jstring getResultString(JNIEnv *env);

    void setResultString(JNIEnv *env, jstring jstringValue1);

//    void destroyNative(JNIEnv *env);
//
//    static void destroy(JNIEnv *env, jobject java_this);

    jobject getHttpPostNative(JNIEnv *env);

    static jobject getHttpPost(JNIEnv *env, jobject java_this);

    void putNative(JNIEnv *env, jstring jstringValue1, jstring jstringValue2);

    static void put(JNIEnv *env, jobject java_this, jstring jstringValue1, jstring jstringValue2);

    jstring getNative(JNIEnv *env, jstring jstringValue1);

    static jstring get(JNIEnv *env, jobject java_this, jstring jstringValue1);

    jstring toJSONStringNative(JNIEnv *env);

    static jstring toJSONString(JNIEnv *env, jobject java_this);

    jstring request(JNIEnv *env, jint jintValue1);

    static void nativeDestroy(JNIEnv *env, jobject java_this);

    static const int BASE = 0;
    static const int INCREMENT = 1;
    static const int HTTP_BIN = BASE + INCREMENT;
    static const int JSON_TEST = HTTP_BIN + INCREMENT;
};

static Network networkInstance;

#endif //APPLICATIONRESOURCES_NETWORK_H
