//
// Created by Christopher Miller on 1/14/17.
//

#include <iostream>
#include <sstream>
#include "EncodedString.h"

using namespace std;

EncodedString::EncodedString() : NativeObject() {}

/**
* This constructor is the loader for the Java instance used to call all of its methods.
* You can construct the EncodedString object how ever you need to here.
*/
EncodedString::EncodedString(JNIEnv *env) : NativeObject(env)
{
    initialize(env);

    thisObj = toJavaObject(env); // THIS IS WHERE YOU INITIALIZE YOUR JAVA OBJECT, YOU MIGHT WANT TO ADD A FEW PARAMETERS BELOW
//    thisObj = env->NewObject(_clazz, getMethod(getCanonicalName())); // CALLS DEFAULT CONSTRUCTOR

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "EncodedString's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}
void EncodedString::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);
    cacheField(env, "encodedString", kTypeString);

    cacheSignature(env, "equals", "(Ljava/lang/Object;)Z");
    cacheSignature(env, "toString", "()Ljava/lang/String;");
    addNativeSignature("decrypt", (void*)&EncodedString::decrypt, "(I)Ljava/lang/String;");

    // addNativeMethod("destroy", (void*)&EncodedString::nativeDestroy, kTypeVoid, NULL);
    registerNativeMethods(env);
}
void EncodedString::mapFields()
{
    mapField("encodedString", kTypeString, &encodedString);
}

jboolean EncodedString::equals(JNIEnv *env, jobject jobjectValue1)
{
    jboolean result = env->CallBooleanMethod(thisObj, getMethod(__FUNCTION__), jobjectValue1);
    JavaExceptionUtils::checkException(env);
    return result;
}

jstring EncodedString::toString(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jstring EncodedString::decryptNative(JNIEnv *env, jint algorithm)
{
    return EncodedString::decrypt(env, thisObj, algorithm);
}

string rot(string s, int r)
{
//    string nonAlphas = "";
//    string nonVisuals = "";

    stringstream ss;
    for (int i = 0; i < s.length(); i++)
    {
        char c = s[i];
        if (c >= 'a' && c <= 'z') ss << (char)((c - 'a' + r) % 26 + 'a');
        else if (c >= 'A' && c <= 'Z') ss << (char)((c - 'A' + r) % 26 + 'A');
        else if (c >= '{' && c <= '~') ss << (char)((c - 'a' + r) % 26 + 'a');
        else if (c >= 'A' && c <= 'Z') ss << (char)((c - 'A' + r) % 26 + 'A');
        else ss << c;
    }
    return ss.str();
}

jstring EncodedString::decrypt(JNIEnv *env, jobject java_this, jint algorithm)
{
    EncodedString *object = gClasses.getNativeInstance<EncodedString>(env, java_this);
    if (object != NULL) {
        std::string conversion = rot(object->encodedString.get(), -1);
        jstring result = env->NewStringUTF(conversion.c_str());
        JavaExceptionUtils::checkException(env);
        return (jstring) result;
    }
    return NULL;
}
