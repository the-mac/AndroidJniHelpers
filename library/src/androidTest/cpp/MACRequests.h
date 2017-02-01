//
// Created by Christopher Miller on 1/16/17.
//

#ifndef APPLICATIONRESOURCES_MACREQUESTS_H
#define APPLICATIONRESOURCES_MACREQUESTS_H

#include <Network.h>
#include <JniHelpers.h>

using namespace std;

class MACRequests : public Network {

protected:
    std::string requestSession;

public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the MACRequests class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", MACRequests);
    }

    MACRequests();

    MACRequests(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

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


    static jobject getInstance(JNIEnv *env, jobject java_this);

    static jobject getHttpPost(JNIEnv *env, jobject java_this);


    static const int BASE = 0;
    static const int INCREMENT = 1;
    static const int HTTP_BIN = BASE + INCREMENT;
    static const int JSON_TEST = HTTP_BIN + INCREMENT;
};

#endif //APPLICATIONRESOURCES_NETWORK_H
