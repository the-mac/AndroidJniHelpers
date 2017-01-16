//
// Created by Christopher Miller on 1/14/17.
//

#ifndef ANDROIDJNI_ENCODEDSTRING_H
#define ANDROIDJNI_ENCODEDSTRING_H

#include "JniHelpers.h"
using namespace spotify::jni;

class EncodedString : public NativeObject {
    jobject thisObj;
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the EncodedString class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", EncodedString);
    }

    EncodedString();

    EncodedString(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

    jboolean equals(JNIEnv *env, jobject jobjectValue1);

    jstring toString(JNIEnv *env);

    jstring nativeDecode(JNIEnv *env);

    static jstring decode(JNIEnv *env, jobject java_this);

public:
    JavaString encodedString;
};

#endif //ANDROIDJNI_ENCODEDSTRING_H
