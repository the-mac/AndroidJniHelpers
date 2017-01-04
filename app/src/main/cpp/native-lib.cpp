#include <jni.h>
#include <string>
#include <iostream>
#include <sstream>
#include <JniHelpers.h>
#include <DecodedString.h>

using namespace std;
using namespace spotify::jni;
ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
    LOG_INFO("Initializing JNI");
    JNIEnv *env = jniHelpersInitialize(jvm);
    if (env == NULL) {
        return -1;
    }

    gClasses.add(env, new DecodedString(env));

    LOG_INFO("Initialization complete");
    return JAVA_VERSION;
}

extern "C"
jstring
Java_us_the_mac_android_jni_helpers_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject thIs) {

    DecodedString *theString = new DecodedString(env);
    theString->encodedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";
    jstring decodedString = theString->decode(env, theString->toJavaObject(env));
    return decodedString;
;
}