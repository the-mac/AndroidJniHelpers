//
// Created by Christopher Miller on 2/12/17.
//

#include "CryptoHelper.h"
CryptoHelper::CryptoHelper() : JavaClass() {}

/**
* Here you can construct the CryptoHelper object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
CryptoHelper::CryptoHelper(JNIEnv *env) : JavaClass(env)
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

    cacheSignature(env, "decrypt", "([B[B)[B");

    registerNativeMethods(env);
}

void CryptoHelper::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}


jobject CryptoHelper::generateSymmetricKey(JNIEnv *env)
{
    jclass _class = env->FindClass("us/the/mac/android/jni/helpers/AndroidJniApp");
    jmethodID method = env->GetStaticMethodID(_class, __FUNCTION__, "()Lus/the/mac/android/jni/helpers/AndroidJniApp;");
    jobject result = env->CallStaticObjectMethod(_class, method);
    JavaExceptionUtils::checkException(env);
    return result;
}

//public static Key generateSymmetricKey() throws Exception {
//
//        byte [] decodedKey = Base64.decodeBase64( getBytes() );
//        SecretKey originalKey = new SecretKeySpec(decodedKey, 0, decodedKey.length, "AES");
//
//        return originalKey;
//}

jbyteArray CryptoHelper::decrypt(JNIEnv *env, jbyteArray keyBytes, jbyteArray valueBytes)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), keyBytes, valueBytes);
    JavaExceptionUtils::checkException(env);
    return (jbyteArray) result;
}
