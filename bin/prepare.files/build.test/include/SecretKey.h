//
// Created by Christopher Miller on 2/17/17.
//

#ifndef APPLICATIONRESOURCES_SECRETKEY_H
#define APPLICATIONRESOURCES_SECRETKEY_H

#include "JniHelpers.h"

class SecretKey : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the SecretKey class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("javax/crypto", SecretKey);
    }
    SecretKey();

    SecretKey(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

};

#endif //APPLICATIONRESOURCES_SECRETKEY_H
