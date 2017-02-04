//
// Created by Christopher Miller on 1/14/17.
//

#include <iostream>
#include <sstream>
#include "EncryptedString.h"

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
//        else if (c >= '{' && c <= '~') ss << (char)((c - 'a' + r) % 26 + 'a');
//        else if (c >= 'A' && c <= 'Z') ss << (char)((c - 'A' + r) % 26 + 'A');
        else ss << c;
    }
    return ss.str();
}

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
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

jstring EncryptedString::decrypt(JNIEnv *env, jobject java_this, jint algorithm)
{
    EncryptedString *object = gClasses.getNativeInstance<EncryptedString>(env, java_this);
    if (object != NULL) {

        std::string conversion;
        switch(algorithm) {
        case INLINE_STRINGS_ALGORITHM:
            conversion = rot(object->encryptedString.get(), -1);
            break;
        case RESOURCE_STRINGS_ALGORITHM:
            conversion = rot(base64(object->encryptedString.get()), -1);
            break;
        }

        jstring result = env->NewStringUTF(conversion.c_str());
        JavaExceptionUtils::checkException(env);
        return result;
    }
    return NULL;
}
