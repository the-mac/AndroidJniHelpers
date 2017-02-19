//
// Created by Christopher Miller on 2/12/17.
//

#ifndef APPLICATIONRESOURCES_CRYPTOHELPER_H
#define APPLICATIONRESOURCES_CRYPTOHELPER_H

#include "JniHelpers.h"

using namespace spotify::jni;

class CryptoHelper : public NativeObject {
    ByteArray byteArray;

public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the CryptoHelper class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", CryptoHelper);
    }
    CryptoHelper();

    CryptoHelper(JNIEnv *env);

//    CryptoHelper(JNIEnv *env, jbyteArray bArray);

    void initialize(JNIEnv *env);

    void mapFields();

    static jobject generateKey(JNIEnv *env, jobject java_this);

    jbyteArray decryptNative(JNIEnv *env, jbyteArray valueBytes);

    jbyteArray decrypt(JNIEnv *env, jobject java_this, jbyteArray valueBytes);

    void setBytes(JNIEnv *env, jbyteArray bytes);

    static void nativeDestroy(JNIEnv *env, jobject java_this);

    jobject thisObj;
};


#endif //APPLICATIONRESOURCES_CRYPTOHELPER_H
