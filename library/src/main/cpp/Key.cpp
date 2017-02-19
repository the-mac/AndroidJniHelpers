//
// Created by Christopher Miller on 2/16/17.
//

#include "Key.h"

Key::Key() : JavaClass() {}

/**
* Here you can construct the Key object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
Key::Key(JNIEnv *env) : JavaClass(env)
{
    initialize(env);

    thisObj = toJavaObject(env); // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "Key's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void Key::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    cacheSignature(env, "getAlgorithm", "()Ljava/lang/String;");
    cacheSignature(env, "getFormat", "()Ljava/lang/String;");
    cacheSignature(env, "getEncoded", "()[B");

    registerNativeMethods(env);
}

void Key::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}

jstring Key::getAlgorithm(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jstring Key::getFormat(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jbyteArray Key::getEncoded(JNIEnv *env)
{
    jbyteArray result = (jbyteArray) env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return result;
}