//
// Created by donnell on 1/20/17.
//

#include "FileOutputStream.h"

FileOutputStream::FileOutputStream() : JavaClass() {}

/**
* Here you can construct the FileOutputStream object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
FileOutputStream::FileOutputStream(JNIEnv *env, jobject stream) : JavaClass(env)
{
    initialize(env);

    thisObj = stream; // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "FileOutputStream's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void FileOutputStream::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    cacheSignature(env, "getChannel", "()Ljava/nio/channels/FileChannel;");

    registerNativeMethods(env);
}

void FileOutputStream::mapFields()
{
    //mapField("encryptedString", kTypeString, &encryptedString);
}

jobject FileOutputStream::write(JNIEnv *env, jobject bytes)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), bytes);
    JavaExceptionUtils::checkException(env);
    return result;
}

void FileOutputStream::close(JNIEnv *env)
{
    env->CallVoidMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
}

