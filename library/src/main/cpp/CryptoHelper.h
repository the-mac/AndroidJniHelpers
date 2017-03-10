//
// Created by Christopher Miller on 2/12/17.
//

#ifndef APPLICATIONRESOURCES_CRYPTOHELPER_H
#define APPLICATIONRESOURCES_CRYPTOHELPER_H
#include "JniHelpers.h"

using spotify::jni::NativeObject;

class CryptoHelper : public NativeObject {
public:
    jbyteArray keyBytes;

    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the CryptoHelper class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", CryptoHelper);
    }
    CryptoHelper();

    CryptoHelper(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

    void setBytes(jbyteArray jbyteArrayValue);

    jobject generateKeyNative(JNIEnv *env);

    static jobject generateKey(JNIEnv *env, jobject java_this);

    jbyteArray decrypt(JNIEnv *env, jbyteArray jbyteArrayValue1, jbyteArray jbyteArrayValue2);

    jstring decrypt(JNIEnv *env, jstring jstringValue1);

    void destroyNative(JNIEnv *env);

};



#endif //APPLICATIONRESOURCES_CRYPTOHELPER_H
