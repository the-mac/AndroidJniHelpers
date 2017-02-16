//
// Created by donnell on 1/20/17.
//

#include "FileInputStream.h"

FileInputStream::FileInputStream() : JavaClass() {}

/**
* Here you can construct the FileInputStream object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
FileInputStream::FileInputStream(JNIEnv *env, jobject stream) : JavaClass(env)
{
    initialize(env);

    thisObj = stream; // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "FileInputStream's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void FileInputStream::initialize(JNIEnv *env)
{
    setClass(env);
//    cacheConstructor(env);

    cacheSignature(env, "<init>", "(Ljava/io/InputStream;)V");

    registerNativeMethods(env);
}

void FileInputStream::mapFields()
{
    //mapField("encryptedString", kTypeString, &encryptedString);
}

//jobject FileInputStream::write(JNIEnv *env, jobject bytes)
//{
//    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), bytes);
//    JavaExceptionUtils::checkException(env);
//    return result;
//}
//
//void FileInputStream::close(JNIEnv *env)
//{
//    env->CallVoidMethod(thisObj, getMethod(__FUNCTION__));
//    JavaExceptionUtils::checkException(env);
//}

