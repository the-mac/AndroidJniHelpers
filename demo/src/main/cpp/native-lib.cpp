#include <jni.h>
#include <string>
#include <EncryptedString.h>
#include <CryptoHelper.h>
#include <JSONObject.h>
#include <Scanner.h>
#include "MainRequests.h"

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
    gClasses.add(env, new MainRequests(env));
    gClasses.add(env, new Network(env));

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

extern "C"
void
Java_us_the_mac_library_demo_androidjni_MainProvider_loadAPIKey(JNIEnv* env, jclass _class) {
    bool debug = true;//ENABLE_LOGGING_DEBUG;//
    if(debug) LOG_WARN("Called Java_us_the_mac_library_demo_androidjni_MainIntentService_loadAPIKey in native-lib");

    MainRequests *network = new MainRequests(env);
    if(!network->hasKey(env)) {

        std::string apiKey = "apiKey";
        EncryptedString es(env);

        jstring native_key = env->NewStringUTF("native_key");
        Scanner s = Scanner(env, es.getFileStream(env, native_key));
        s.useDelimiter(env, "\\A");

        std::string nativeKey = env->GetStringUTFChars(s.next(env), JNI_FALSE);
        if(debug) LOG_WARN("The nativeKey string is %s for %s", nativeKey.c_str(), es.getCanonicalName());
        network->putNative(apiKey, nativeKey);

        std::string response = env->GetStringUTFChars(network->request(env, MainRequests::HTTP_BIN), JNI_FALSE);
        if(debug) LOG_WARN("The response string is %s for %s", response.c_str(), network->getCanonicalName());

        int length = response.length();
        if(debug) LOG_WARN("The response string length is %d for %s", length, network->getCanonicalName());

        JSONObject jsonObject(env, response);
        JSONObject data(env, jsonObject.getString(env, "data"));

        jstring resultingToken = data.getString(env, apiKey);
        network->setKey(env, resultingToken);
    }
}
