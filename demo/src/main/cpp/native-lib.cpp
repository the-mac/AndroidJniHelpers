#include <jni.h>
#include <string>
#include <EncryptedString.h>
#include <CryptoHelper.h>

using namespace spotify::jni;
ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
    bool debug = true;//ENABLE_LOGGING_DEBUG;//
    if(debug) LOG_WARN("Called JNI_OnLoad in native-lib");
    JNIEnv *env = jniHelpersInitialize(jvm);
    if (env == NULL) {
        return -1;
    }

    gClasses.add(env, new EncryptedString(env));
    gClasses.add(env, new CryptoHelper(env));

    if(debug) LOG_WARN("Called JNI_OnLoad Completely in native-lib");
    return JAVA_VERSION;
}

extern "C"
jstring
Java_us_the_mac_library_demo_androidjni_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject java_this) {
    bool debug = true;//ENABLE_LOGGING_DEBUG;//
    if(debug) LOG_WARN("Called Java_us_the_mac_library_demo_androidjni_MainActivity_stringFromJNI in native-lib");

    EncryptedString object(env);
    object.encryptedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";
    return object.decryptNative(env, EncryptedString::INLINE_STRINGS_ALGORITHM);

}

extern "C"
jstring
Java_us_the_mac_android_jni_helpers_AndroidJniApp_getS(JNIEnv* env, jclass _class, jint content) {
    bool debug = true;//ENABLE_LOGGING_DEBUG;//
    if(debug) LOG_WARN("Called Java_us_the_mac_android_jni_helpers_AndroidJniApp_getS in native-lib");

    EncryptedString es = EncryptedString(env);
    jstring stringResource = EncryptedString::getS(env, content);
    es.encryptedString = env->GetStringUTFChars(stringResource, JNI_FALSE);

    return es.decryptNative(env, EncryptedString::RESOURCE_STRINGS_ALGORITHM);
}
