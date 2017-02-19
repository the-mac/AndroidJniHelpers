//
// Created by Christopher Miller on 2/17/17.
//

#include "SecretKey.h"

SecretKey::SecretKey() : JavaClass() {}

/**
* Here you can construct the SecretKey object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
SecretKey::SecretKey(JNIEnv *env) : JavaClass(env)
{
    initialize(env);

    thisObj = toJavaObject(env); // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "SecretKey's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void SecretKey::initialize(JNIEnv *env)
{
    setClass(env);

    cacheSignature(env, "<init>", "([BIILjava/lang/String;)V");

    registerNativeMethods(env);
}

void SecretKey::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}
