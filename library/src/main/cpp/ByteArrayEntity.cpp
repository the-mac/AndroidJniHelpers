//
// Created by donnell on 1/18/17.
//

#include "ByteArrayEntity.h"

/**
* Here you can construct the ByteArrayEntity object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
ByteArrayEntity::ByteArrayEntity(JNIEnv *env, jbyteArray array) : JavaClass(env)
{
    initialize(env);

    // WE HAVE ADDED A jbyteArray array PARAMETER TO THE 'ByteArrayEntity' constructor
    thisObj = env->NewObject(_clazz, getMethod("<init>"), array);

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "ByteArrayEntity's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void ByteArrayEntity::initialize(JNIEnv *env)
{
    setClass(env);

    cacheSignature(env, "<init>", "([B)V");

    registerNativeMethods(env);
}

void ByteArrayEntity::mapFields()
{
    //mapField("encodedString", kTypeString, &encodedString);
}
