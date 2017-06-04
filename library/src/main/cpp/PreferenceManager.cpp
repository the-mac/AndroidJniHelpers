//
// Created by donnell on 1/20/17.
//

#include "PreferenceManager.h"

/**
* Here you can construct the PreferenceManager object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
PreferenceManager::PreferenceManager(JNIEnv *env) : JavaClass(env)
{
    initialize(env);

    thisObj = toJavaObject(env); // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "PreferenceManager's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void PreferenceManager::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);

    cacheSignature(env, "getSharedPreferences", "()Landroid/content/SharedPreferences;");
    cacheStaticSignature(env, "getDefaultSharedPreferences", "(Landroid/content/Context;)Landroid/content/SharedPreferences;");

    registerNativeMethods(env);
}

void PreferenceManager::mapFields()
{
    //mapField("encryptedString", kTypeString, &encryptedString);
}

jobject PreferenceManager::getDefaultSharedPreferences(JNIEnv *env, jobject jobjectValue1)
{
    jclass _class = env->FindClass("android/preference/PreferenceManager");
    jmethodID method = env->GetStaticMethodID(_class, __FUNCTION__, "(Landroid/content/Context;)Landroid/content/SharedPreferences;");
    jobject result = env->CallStaticObjectMethod(_class, method, jobjectValue1);
    JavaExceptionUtils::checkException(env);
    return result;
}
