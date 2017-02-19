//
// Created by Christopher Miller on 2/12/17.
//

#include "CryptoHelper.h"
#include "Base64.h"
#include "SecretKey.h"
#include "SecretKeySpec.h"

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
/**
* Here you can construct the CryptoHelper object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
//CryptoHelper::CryptoHelper(JNIEnv *env, jbyteArray bArray) : NativeObject(env)
//{
//    initialize(env);
//
//    byteArray = ByteArray(env, bArray);
//
//    thisObj = toJavaObject(env); // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT
//
//    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
//    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD
//
//    if (thisObj == NULL) {
//        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
//                                                 "CryptoHelper's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
//    }
//}

void CryptoHelper::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

//    cacheSignature(env, "decrypt", "([B[B)[B");
    addNativeMethod("destroy", (void*)&CryptoHelper::nativeDestroy, kTypeVoid, NULL);
    addNativeSignature("generateKey", (void*)&CryptoHelper::generateKey, "()Ljava/security/Key;");

    registerNativeMethods(env);
}

void CryptoHelper::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}


jobject CryptoHelper::generateKey(JNIEnv *env, jobject java_this)
{
    CryptoHelper *object = gClasses.getNativeInstance<CryptoHelper>(env, java_this);

    if (object != NULL) {
//
//        // us.the.mac.android.jni.helpers.CryptoHelper
//        // us/the/mac/android/jni/helpers/CryptoHelper
////    jclass _class = env->FindClass("us/the/mac/android/jni/helpers/AndroidJniApp");
//
//        // java.security.Key
//        // java/security/Key
////    jmethodID method = env->GetStaticMethodID(_class, __FUNCTION__, "()Lus/the/mac/android/jni/helpers/AndroidJniApp;");
//
//        jobject result = NULL;//env->CallStaticObjectMethod(_class, method);
//        JavaExceptionUtils::checkException(env);
//        return result;

//        byte [] decodedKey = Base64.decodeBase64( getBytes() );
//        ByteArray encodedKey = ByteArray(env, JavaString(object->getKey(env, NATIVE_STRINGS_ALGORITHM)).toByteArray(env));



        ByteArray decodedKey = ByteArray(env, Base64::decodeBase64( env, object->byteArray.toJavaByteArray(env) ));
        jbyteArray array = decodedKey.toJavaByteArray(env);
        jint start = (jint) 0;
        jint length = (jint) decodedKey.size();
        SecretKeySpec originalKey = SecretKeySpec(env, array, start, length);

        return originalKey.thisObj;

    }
    return NULL;
}

//public static Key generateKey() throws Exception {
//
//        byte [] decodedKey = Base64.decodeBase64( getBytes() );
//        SecretKey originalKey = new SecretKeySpec(decodedKey, 0, decodedKey.length, "AES");
//
//        return originalKey;
//}


jbyteArray CryptoHelper::decryptNative(JNIEnv *env, jbyteArray valueBytes)
{
    return CryptoHelper::decrypt(env, thisObj, valueBytes);
}
jbyteArray CryptoHelper::decrypt(JNIEnv *env, jobject java_this, jbyteArray valueBytes)
{
    jbyteArray keyBytes = byteArray.toJavaByteArray(env);
    jobject result = env->CallObjectMethod(java_this, getMethod(__FUNCTION__), keyBytes, valueBytes);
    JavaExceptionUtils::checkException(env);
    return (jbyteArray) result;
}

void CryptoHelper::setBytes(JNIEnv *env, jbyteArray bytes) {
    byteArray = ByteArray(env, bytes);
}

void CryptoHelper::nativeDestroy(JNIEnv *env, jobject java_this) {
    CryptoHelper *object = gClasses.getNativeInstance<CryptoHelper>(env, java_this);
    if (object != NULL) {
        // NativeObject *native_object = dynamic_cast<NativeObject*>(object);
//        object->destroy(env, java_this);
    }
}
