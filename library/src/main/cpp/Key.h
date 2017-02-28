//
// Created by Christopher Miller on 2/16/17.
//

#ifndef APPLICATIONRESOURCES_KEY_H
#define APPLICATIONRESOURCES_KEY_H

#include "JniHelpers.h"

class Key : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the Key class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("java/security", Key);
    }
    Key();

    Key(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

    jstring getAlgorithm(JNIEnv *env);

    jstring getFormat(JNIEnv *env);

    jbyteArray getEncoded(JNIEnv *env);

};


#endif //APPLICATIONRESOURCES_KEY_H
