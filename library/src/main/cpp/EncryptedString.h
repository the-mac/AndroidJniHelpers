//
// Created by Christopher Miller on 1/14/17.
//

#ifndef ANDROIDJNI_ENRYPTEDSTRING_H
#define ANDROIDJNI_ENRYPTEDSTRING_H

#include "JniHelpers.h"

using namespace spotify::jni;

using namespace std;

class EncryptedString : public NativeObject {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the EncryptedString class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", EncryptedString);
    }

    EncryptedString();

    EncryptedString(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

    jboolean equals(JNIEnv *env, jobject jobjectValue1);

    jstring toString(JNIEnv *env);

    jstring decryptNative(JNIEnv *env, jint algorithm);

    virtual string getKey(JNIEnv *env, jint algorithm);

//    virtual string getCache(JNIEnv *env); // USE FOR LOCAL FILE

    /**
    * The getBytes method is used to get the decrypt key
    * @return AES (Advanced Encryption Standard) key bytes for decryption.
    */
    static jbyteArray getBytes(JNIEnv *env, jobject java_this);

    static jobject getFileStream(JNIEnv *env, jstring fileName);

    static jstring decrypt(JNIEnv *env, jobject java_this, jint algorithm);

    static jstring getS(JNIEnv *env, jint index);

public:
    JavaString encryptedString;

    static const int BASE = 0;
    static const int INCREMENT = 1;

    static const int INLINE_STRINGS_ALGORITHM = BASE + INCREMENT;
    static const int RESOURCE_STRINGS_ALGORITHM = INLINE_STRINGS_ALGORITHM + INCREMENT;
    static const int NATIVE_STRINGS_ALGORITHM = RESOURCE_STRINGS_ALGORITHM + INCREMENT;
};

#endif //ANDROIDJNI_ENCODEDSTRING_H
