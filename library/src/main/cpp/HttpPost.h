//
// Created by donnell on 1/18/17.
//

#ifndef APPLICATIONRESOURCES_HTTPPOST_H
#define APPLICATIONRESOURCES_HTTPPOST_H

#include "JniHelpers.h"
#include "ByteArrayEntity.h"
#include <string>

using namespace std;

class HttpPost : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the HttpPost class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("org/apache/http/client/methods", HttpPost);
    }
    HttpPost();

    HttpPost(JNIEnv *env, string url);

    void initialize(JNIEnv *env);

    void mapFields();

//    jstring getMethod(JNIEnv *env);
//
//    jmethodID getMethod(const char *method_name) const;

    void setEntity(JNIEnv *env, ByteArrayEntity entity);

    void setHeader(JNIEnv *env, string key, string value);

};

#endif //APPLICATIONRESOURCES_HTTPPOST_H
