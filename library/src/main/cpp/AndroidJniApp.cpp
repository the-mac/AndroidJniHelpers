//
// Created by donnell on 1/20/17.
//

#include <EncryptedString.h>
#include "AndroidJniApp.h"
AndroidJniApp::AndroidJniApp() : JavaClass() {}

/**
* Here you can construct the AndroidJniApp object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
AndroidJniApp::AndroidJniApp(JNIEnv *env) : JavaClass(env)
{
    initialize(env);

    thisObj = Instance(env); // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "AndroidJniApp's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

/**
* Here you can construct the AndroidJniApp object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
AndroidJniApp::AndroidJniApp(JNIEnv *env, jobject context) : JavaClass(env)
{
    initialize(env);

    thisObj = context; // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "AndroidJniApp's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void AndroidJniApp::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    cacheStaticSignature(env, "Instance", "()Lus/the/mac/android/jni/helpers/AndroidJniApp;");
    cacheSignature(env, "getString", "(I)Ljava/lang/String;");
    cacheSignature(env, "getFilesDir", "()Ljava/io/File;");
    cacheSignature(env, "openFileOutput", "(Ljava/lang/String;I)Ljava/io/FileOutputStream;");
    cacheSignature(env, "openFileInput", "(Ljava/lang/String;)Ljava/io/FileInputStream;");
//    addNativeSignature("decryptString", (void *) AndroidJniApp::decryptString, "(I)Ljava/lang/String;");

    registerNativeMethods(env);
}

void AndroidJniApp::mapFields()
{
    //mapField("encryptedString", kTypeString, &encryptedString);
}

JavaClass* AndroidJniApp::getPersistedInstance(JNIEnv *env, jobject javaThis) {
    return gClasses.getNativeInstance<AndroidJniApp>(env, javaThis);
}

jobject AndroidJniApp::Instance(JNIEnv *env)
{
    jclass _class = env->FindClass("us/the/mac/android/jni/helpers/AndroidJniApp");
    jmethodID method = env->GetStaticMethodID(_class, __FUNCTION__, "()Lus/the/mac/android/jni/helpers/AndroidJniApp;");
    jobject result = env->CallStaticObjectMethod(_class, method);
    JavaExceptionUtils::checkException(env);
    return result;
}

jstring AndroidJniApp::getFilesDir(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jobject AndroidJniApp::openFileOutput(JNIEnv *env, jstring path, jint mode)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), path, mode);
    JavaExceptionUtils::checkException(env);
    return result;
}


jobject AndroidJniApp::openFileInput(JNIEnv *env, jstring path)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), path);
    JavaExceptionUtils::checkException(env);
    return result;
}

jstring AndroidJniApp::decryptStringNative(JNIEnv *env, jint resource) { return AndroidJniApp::decryptString(
            env, thisObj, resource); }


jstring AndroidJniApp::decryptString(JNIEnv *env, jobject java_this, jint resourceId) {
    AndroidJniApp *object = gClasses.getNativeInstance<AndroidJniApp>(env, java_this);

    if (object != NULL) {

//        jobject stringResource = env->CallObjectMethod(object->thisObj, object->getMethod("getString"), resourceId);
//        JavaExceptionUtils::checkException(env);
//
//        EncryptedString es = EncryptedString(env);
//        es.encryptedString = env->GetStringUTFChars((jstring) stringResource, JNI_FALSE);
//        JavaExceptionUtils::checkException(env);
//
//        return es.decryptNative(env, EncryptedString::RESOURCE_STRINGS_ALGORITHM);
    }
    return NULL;
}