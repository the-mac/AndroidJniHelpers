//
// Created by donnell on 1/20/17.
//

#include "SharedPreferences.h"
#include "PreferenceManager.h"

SharedPreferences::SharedPreferences() : JavaClass() {}

/**
* Here you can construct the SharedPreferences object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
SharedPreferences::SharedPreferences(JNIEnv *env, jobject context) : JavaClass(env)
{
    initialize(env);

    thisObj = PreferenceManager::getDefaultSharedPreferences(env, context);

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "SharedPreferences's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void SharedPreferences::initialize(JNIEnv *env)
{
    setClass(env);

    cacheSignature(env, "getAll", "()Ljava/util/Map;");
    cacheSignature(env, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    cacheSignature(env, "getBoolean", "(Ljava/lang/String;Z)Z");
    cacheSignature(env, "contains", "(Ljava/lang/String;)Z");
    cacheSignature(env, "edit", "()Landroid/content/SharedPreferences$Editor;");

    registerNativeMethods(env);
}

void SharedPreferences::mapFields()
{
    //mapField("encryptedString", kTypeString, &encryptedString);
}

jstring SharedPreferences::getString(JNIEnv *env, jstring jstringValue1, jstring jstringValue2)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), jstringValue1, jstringValue2);
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jboolean SharedPreferences::getBoolean(JNIEnv *env, jstring jstringValue1, jboolean jbooleanValue2)
{
    jboolean result = env->CallBooleanMethod(thisObj, getMethod(__FUNCTION__), jstringValue1, jbooleanValue2);
    JavaExceptionUtils::checkException(env);
    return result;
}

jboolean SharedPreferences::contains(JNIEnv *env, jstring jstringValue1)
{
    jboolean result = env->CallBooleanMethod(thisObj, getMethod(__FUNCTION__), jstringValue1);
    JavaExceptionUtils::checkException(env);
    return result;
}

jobject SharedPreferences::edit(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return result;
}

SharedPreferences::Editor::Editor() : JavaClass() {}

/**
* Here you can construct the Editor object how ever you need to, 
* as well as load signatures for the Java instance method calls.
*/
SharedPreferences::Editor::Editor(JNIEnv *env, jobject editor) : JavaClass(env)
{
    initialize(env);

    thisObj = editor; // THIS IS WHERE WE INITIALIZE YOUR JAVA OBJECT

    // thisObj = env->NewObject(_clazz, getMethod("<init>"));
    // YOU MAY WANT TO ADD A FEW PARAMETERS TO THE 'NewObject' EXAMPLE INSTEAD

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "Editor's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}

void SharedPreferences::Editor::initialize(JNIEnv *env)
{
    setClass(env);

    cacheSignature(env, "putString", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
    cacheSignature(env, "putBoolean", "(Ljava/lang/String;Z)Landroid/content/SharedPreferences$Editor;");
    cacheSignature(env, "remove", "(Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
    cacheSignature(env, "clear", "()Landroid/content/SharedPreferences$Editor;");
    cacheSignature(env, "commit", "()Z");

    registerNativeMethods(env);
}

void SharedPreferences::Editor::mapFields()
{
    //mapField("encryptedString", kTypeString, &encryptedString);
}

jobject SharedPreferences::Editor::putString(JNIEnv *env, jstring jstringValue1, jstring jstringValue2)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), jstringValue1, jstringValue2);
    JavaExceptionUtils::checkException(env);
    return result;
}

jobject SharedPreferences::Editor::putBoolean(JNIEnv *env, jstring jstringValue1, jboolean jbooleanValue2)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), jstringValue1, jbooleanValue2);
    JavaExceptionUtils::checkException(env);
    return result;
}

jobject SharedPreferences::Editor::remove(JNIEnv *env, jstring jstringValue1)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__), jstringValue1);
    JavaExceptionUtils::checkException(env);
    return result;
}

jobject SharedPreferences::Editor::clear(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return result;
}

jboolean SharedPreferences::Editor::commit(JNIEnv *env)
{
    jboolean result = env->CallBooleanMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return result;
}