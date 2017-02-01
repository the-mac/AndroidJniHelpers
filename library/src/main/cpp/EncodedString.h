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

<<<<<<< HEAD
    jstring decryptNative(JNIEnv *env, jint algorithm);
=======
    jstring decodeNative(JNIEnv *env);
>>>>>>> 1ed9d31c8a05ab45a7aff45d28ef2820b22a2583

    static jstring decrypt(JNIEnv *env, jobject java_this, jint algorithm);

public:
    JavaString encodedString;

    static const int BASE = 0;
    static const int INCREMENT = 1;
    static const int INLINE_STRINGS_ALGORITHM = BASE + INCREMENT;
    static const int RESOURCE_STRINGS_ALGORITHM = INLINE_STRINGS_ALGORITHM + INCREMENT;
};

#endif //ANDROIDJNI_ENCODEDSTRING_H
