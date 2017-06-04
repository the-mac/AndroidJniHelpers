#include <jni.h>
#include <string>
#include <JniHelpers.h>
#include "JniHelpersTest.h"
#include "JavaStringTest.h"
#include "NativeObjectTest.h"
#include "EncryptedStringTest.h"
#include "JavaClassTest.h"
#include "NetworkTest.h"
#include "ByteArrayTest.h"

#include <EncryptedString.h>
#include <CryptoHelper.h>
#include <MACRequests.h>

using namespace spotify::jni;

ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
    bool debug = ENABLE_LOGGING_DEBUG;
    if(debug) LOG_WARN("Called JNI_OnLoad in test-helper-lib");
    JNIEnv *env = jniHelpersInitialize(jvm);
    if (env == NULL) {
        return -1;
    }

    // BASIC JAVA TESTS
    gClasses.add(env, new ByteArrayTest(env));
    gClasses.add(env, new JavaClassTest(env));
    gClasses.add(env, new NativeObjectTest(env));

    // STRING TESTS
    gClasses.add(env, new JavaStringTest(env));
    gClasses.add(env, new EncryptedStringTest(env));
    gClasses.add(env, new EncryptedString(env));
    gClasses.add(env, new CryptoHelper(env));

    // NETWORK TESTS
    gClasses.add(env, new NetworkTest(env));
    gClasses.add(env, new Network(env));
    gClasses.add(env, new MACRequests(env));

    if(debug) LOG_WARN("Initialization complete");
    return JAVA_VERSION;
}
extern "C"
jstring
Java_us_the_mac_android_jni_helpers_AndroidJniApp_getEncrypted(JNIEnv* env, jclass _class, jint content) {
    return EncryptedString::getS(env, content);
}
extern "C"
jstring
Java_us_the_mac_android_jni_helpers_AndroidJniApp_getS(JNIEnv* env, jclass _class, jint content) {

    EncryptedString es = EncryptedString(env);
    jstring stringResource = EncryptedString::getS(env, content);
    es.encryptedString = env->GetStringUTFChars(stringResource, JNI_FALSE);

    return es.decryptNative(env, EncryptedString::RESOURCE_STRINGS_ALGORITHM);
}