//
// Created by Christopher Miller on 2/12/17.
//

#include <cstring>
#include "CryptoHelper.h"
#include "SecretKeySpec.h"
#include "Base64.h"
#include "Scanner.h"

CryptoHelper::CryptoHelper() : NativeObject() {}

/**
* Here you can construct the CryptoHelper object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
CryptoHelper::CryptoHelper(JNIEnv *env) : NativeObject(env)
{
    initialize(env);

    thisObj = toJavaObject(env); // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "CryptoHelper's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void CryptoHelper::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    addNativeSignature("generateKey", (void*)&CryptoHelper::generateKey, "()Ljava/security/Key;");
//    cacheSignature(env, "decrypt", "([B[B)[B");
    cacheSignature(env, "decrypt", "(Ljava/lang/String;)Ljava/lang/String;");

    registerNativeMethods(env);
}

void CryptoHelper::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}

void CryptoHelper::setBytes(jbyteArray jbyteArrayValue)
{
    keyBytes = jbyteArrayValue;
}

jobject CryptoHelper::generateKeyNative(JNIEnv *env) { return CryptoHelper::generateKey(env, thisObj); }

jobject CryptoHelper::generateKey(JNIEnv *env, jobject java_this) {
    CryptoHelper *object = gClasses.getNativeInstance<CryptoHelper>(env, java_this);

    if (object != NULL)
    {
//        const char bytes[] = {
//            "FVDwFK1liUGl7hDRZLjHBQ==\n"
//        };
//
//        int size = std::strlen(bytes);
//
//        jbyte *data = (jbyte *) bytes;
//        jbyteArray keyBytes = env->NewByteArray(size);
//        env->SetByteArrayRegion(keyBytes, 0, size, data);
//
//        jbyteArray decodedKeyBytes = Base64::decodeBase64(env, keyBytes);

        jbyteArray decodedKeyBytes = Base64::decodeBase64(env, object->keyBytes);

        jint offset = (jint) 0;
        jint length = env->GetArrayLength(decodedKeyBytes);
        jstring algorithm = env->NewStringUTF("AES");

        SecretKeySpec *originalKey = new SecretKeySpec(env, decodedKeyBytes, offset, length, algorithm);
        return originalKey->thisObj;
    }

    return NULL;
}


jbyteArray CryptoHelper::decrypt(JNIEnv *env, jbyteArray jbyteArrayValue1, jbyteArray jbyteArrayValue2)
{
    jbyteArray result = (jbyteArray) env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), jbyteArrayValue1, jbyteArrayValue2);
    JavaExceptionUtils::checkException(env);
    return result;
}

jstring CryptoHelper::decrypt(JNIEnv *env, jstring jstringValue1)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), jstringValue1);
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}