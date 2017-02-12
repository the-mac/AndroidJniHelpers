//
// Created by Christopher Miller on 2/12/17.
//

#ifndef APPLICATIONRESOURCES_CRYPTOHELPER_H
#define APPLICATIONRESOURCES_CRYPTOHELPER_H

#include "JniHelpers.h"

class CryptoHelper : public JavaClass {
    jobject thisObj;

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

    void initialize(JNIEnv *env);

    void mapFields();

    jbyteArray decrypt(JNIEnv *env, jbyteArray keyBytes, jbyteArray valueBytes);

};


#endif //APPLICATIONRESOURCES_CRYPTOHELPER_H
