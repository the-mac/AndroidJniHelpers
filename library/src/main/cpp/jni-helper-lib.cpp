#include <jni.h>
#include <string>
#include <iostream>
#include <sstream>
#include <JniHelpers.h>
#include <EncryptedString.h>
#include <CryptoHelper.h>

using namespace std;
using namespace spotify::jni;
ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
    bool debug = true;//ENABLE_LOGGING_DEBUG;//

    if(debug) LOG_WARN("Called JNI_OnLoad in jni-helper-lib");
    JNIEnv *env = jniHelpersInitialize(jvm);
    if (env == NULL) {
        return -1;
    }

    gClasses.add(env, new CryptoHelper(env));
    gClasses.add(env, new EncryptedString(env));

    if(debug) LOG_WARN("Called JNI_OnLoad Completely in jni-helper-lib");
    return JAVA_VERSION;
}
extern "C"
jstring
Java_us_the_mac_android_jni_helpers_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject thIs) {
    bool debug = true;//ENABLE_LOGGING_DEBUG;//
    if(debug) LOG_WARN("Called Java_us_the_mac_android_jni_helpers_MainActivity_stringFromJNI in jni-helper-lib");

    EncryptedString *theString = new EncryptedString(env);
    theString->encryptedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";
    jstring decryptedString = theString->decryptNative(env, EncryptedString::INLINE_STRINGS_ALGORITHM);
    return decryptedString;
}
extern "C"
jstring
Java_us_the_mac_android_jni_helpers_AndroidJniApp_getS(JNIEnv* env, jclass _class, jint content) {
    bool debug = true;//ENABLE_LOGGING_DEBUG;//
    if(debug) LOG_WARN("Called Java_us_the_mac_android_jni_helpers_AndroidJniApp_getS in jni-helper-lib");

    EncryptedString es = EncryptedString(env);
    jstring stringResource = EncryptedString::getS(env, content);
    es.encryptedString = env->GetStringUTFChars(stringResource, JNI_FALSE);

    return es.decryptNative(env, EncryptedString::NATIVE_STRINGS_ALGORITHM);
}