//
// Created by Christopher Miller on 2/16/17.
//

#ifndef APPLICATIONRESOURCES_BASE64_H
#define APPLICATIONRESOURCES_BASE64_H

#include "JniHelpers.h"

class Base64 : public JavaClass {
    jobject thisObj;
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the Base64 class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("org/apache/commons/codec/binary", Base64);
    }
    /**
    * This facsimile of the Java method java.lang.Object.getClass() is used to maintain
    * the Jni Helper's static relationship to the Base64 class defined in Java.
    */
    static jclass getClass(JNIEnv *env) {
        return env->FindClass(MAKE_CANONICAL_NAME("org/apache/commons/codec/binary", Base64));
    }
    Base64();

    Base64(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

//    jobject android.util.Base64(JNIEnv *env);

    static jbyteArray decodeBase64(JNIEnv *env, jbyteArray contents);

//    static jbyteArray decode(JNIEnv *env, jstring jstringValue1, jint jintValue2);
//
//    static jbyteArray decode(JNIEnv *env, jstring jstringValue1, jint jintValue2);

    static jstring encodeToString(JNIEnv *env, jbyteArray jbyteArrayValue1, jint jintValue2);

//    static jstring encodeToString(JNIEnv *env, jbyteArray jbyteArrayValue1, jint jintValue2);

    static jbyteArray encode(JNIEnv *env, jbyteArray jbyteArrayValue1, jint jintValue2);

//    static jbyteArray encode(JNIEnv *env, jbyteArray jbyteArrayValue1, jint jintValue2);

};


#endif //APPLICATIONRESOURCES_BASE64_H
