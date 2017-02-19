//
// Created by Christopher Miller on 2/17/17.
//

#ifndef APPLICATIONRESOURCES_SECRETKEYSPEC_H
#define APPLICATIONRESOURCES_SECRETKEYSPEC_H


#include "ByteArray.h"
#include "JniHelpers.h"

class SecretKeySpec : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the SecretKeySpec class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("javax/crypto/spec", SecretKeySpec);
    }
    SecretKeySpec();

    SecretKeySpec(JNIEnv *env, jbyteArray array, jint start, jint end);

    void initialize(JNIEnv *env);

    void mapFields();

    jstring getAlgorithm(JNIEnv *env);

    jstring getFormat(JNIEnv *env);

    jbyteArray getEncoded(JNIEnv *env);

    jint hashCode(JNIEnv *env);

    jboolean equals(JNIEnv *env, jobject jobjectValue1);

//    SecretKeySpec(spotify::jni::ByteArray array, int i, int param, const char string[4]);
    jobject thisObj;
};


#endif //APPLICATIONRESOURCES_SECRETKEYSPEC_H
