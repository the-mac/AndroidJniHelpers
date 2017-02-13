//
// Created by Christopher Miller on 1/14/17.
//

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include "EncryptedString.h"
#include "Scanner.h"
#include "File.h"

using namespace std;

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";

EncryptedString::EncryptedString() : NativeObject() {}

/**
* This constructor is the loader for the Java instance used to call all of its methods.
* You can construct the EncryptedString object how ever you need to here.
*/
EncryptedString::EncryptedString(JNIEnv *env) : NativeObject(env)
{
    initialize(env);

    thisObj = toJavaObject(env); // THIS IS WHERE YOU INITIALIZE YOUR JAVA OBJECT, YOU MIGHT WANT TO ADD A FEW PARAMETERS BELOW
//    thisObj = env->NewObject(_clazz, getMethod(getCanonicalName())); // CALLS DEFAULT CONSTRUCTOR

    if (thisObj == NULL) {
        JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                 "EncryptedString's thisObj variable not intialized, methods of this class use the thisObj Java instance.");
    }
}
void EncryptedString::initialize(JNIEnv *env)
{
    setClass(env);
    cacheConstructor(env);
    cacheField(env, "encryptedString", kTypeString);

    cacheSignature(env, "equals", "(Ljava/lang/Object;)Z");
    cacheSignature(env, "toString", "()Ljava/lang/String;");
    cacheSignature(env, "getFilesDir", "()Ljava/lang/String;");
    addNativeSignature("decrypt", (void*)&EncryptedString::decrypt, "(I)Ljava/lang/String;");

    // addNativeMethod("destroy", (void*)&EncryptedString::nativeDestroy, kTypeVoid, NULL);
    registerNativeMethods(env);
}
void EncryptedString::mapFields()
{
    mapField("encryptedString", kTypeString, &encryptedString);
}

jboolean EncryptedString::equals(JNIEnv *env, jobject jobjectValue1)
{
    jboolean result = env->CallBooleanMethod(thisObj, getMethod(__FUNCTION__), jobjectValue1);
    JavaExceptionUtils::checkException(env);
    return result;
}

jstring EncryptedString::toString(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

jstring EncryptedString::decryptNative(JNIEnv *env, jint algorithm)
{
    return EncryptedString::decrypt(env, thisObj, algorithm);
}

jbyteArray EncryptedString::getBytes(JNIEnv *env, jobject java_this) {
    EncryptedString *object = gClasses.getNativeInstance<EncryptedString>(env, java_this);

    if (object != NULL) {
        return JavaString(object->getKey(env, NATIVE_STRINGS_ALGORITHM)).toByteArray(env);

//        jstring key = env->NewStringUTF("key_file");
//        Scanner s = Scanner(env, File(env, object->getFilesDir(env), key));
//        s.useDelimiter(env, "\\A");

//        const char *certificate = env->GetStringUTFChars(s.next(env), JNI_FALSE);
        const char *certificate = object->getKey(env, NATIVE_STRINGS_ALGORITHM).c_str();
        int size = std::strlen(certificate);

        jbyte *data = (jbyte *) certificate;
        jbyteArray array = env->NewByteArray(size);
        env->SetByteArrayRegion(array, 0, size, data);
        return array;
    }
    return NULL;
}
string rot(string s, int r)
{
    stringstream ss;
    for (int i = 0; i < s.length(); i++)
    {
        char c = s[i];
        if (c >= 'a' && c <= 'z') ss << (char)((c - 'a' + r) % 26 + 'a');
        else if (c >= 'A' && c <= 'Z') ss << (char)((c - 'A' + r) % 26 + 'A');
        else ss << c;
    }
    return ss.str();
}

string base64(string const& in)
{
    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (char c : in) {
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb>=0) {
            out.push_back(char((val>>valb)&0xFF));
            valb-=8;
        }
    }
    return out;
}

char key_map(string const& k, char const& in) {
    int position = k.length() / 2;
    std::size_t index = k.find(in);
    return (char) k[position + index];
}

string substitution(string const& k, string const& in)
{
    std::string out;
    int length = in.length();
    for (int i = 0; i < length; i++)
    {
        char o = in[i];
        char r = o;

        if (o >= 'a' && o <= 'z' || o >= 'A' && o <= 'Z')
            r = key_map(k, o);

        out.push_back(r);
    }
    return out;
}

jstring EncryptedString::getFilesDir(JNIEnv *env)
{
    jobject result = env->CallObjectMethod(thisObj, getMethod(__FUNCTION__));
    JavaExceptionUtils::checkException(env);
    return (jstring) result;
}

string EncryptedString::getKey(JNIEnv *env, jint algorithm)
{
    switch(algorithm) {
    case RESOURCE_STRINGS_ALGORITHM:
        return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    case NATIVE_STRINGS_ALGORITHM:
        jstring key = env->NewStringUTF("native_key");
        Scanner s = Scanner(env, File(env, getFilesDir(env), key));
        s.useDelimiter(env, "\\A");
        return env->GetStringUTFChars(s.next(env), JNI_FALSE);
    }
}

jstring EncryptedString::decrypt(JNIEnv *env, jobject java_this, jint algorithm)
{
    EncryptedString *object = gClasses.getNativeInstance<EncryptedString>(env, java_this);
    if (object != NULL) {

        std::string key;
        std::string conversion;

        switch(algorithm) {
        case INLINE_STRINGS_ALGORITHM:
            conversion = rot(object->encryptedString.get(), -1);
            break;
        case RESOURCE_STRINGS_ALGORITHM:
            key = object->getKey(env, RESOURCE_STRINGS_ALGORITHM);
            conversion = substitution(key, base64(object->encryptedString.get()));
            break;//return object->encryptedString.toJavaString(env);//
        case NATIVE_STRINGS_ALGORITHM:
            jbyteArray bytes = object->crypto.decrypt(env, object->getBytes(env, java_this), object->encryptedString.toByteArray(env));
            conversion = JavaString(env, bytes).get();
            break;
        }

        jstring result = env->NewStringUTF(conversion.c_str());
        JavaExceptionUtils::checkException(env);
        return result;
    }
    return NULL;
}