#include <jni.h>
#include <string>
#include <JniHelpers.h>
#include "JniHelpersTest.h"
#include "JavaStringTest.h"
#include "NativeObjectTest.h"
#include "EncryptedStringTest.h"
#include "JavaClassTest.h"
#include "NetworkTest.h"

#include <EncryptedString.h>
#include <CryptoHelper.h>
#include <MACRequests.h>

using namespace spotify::jni;

ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
    LOG_INFO("Initializing JNI");
    JNIEnv *env = jniHelpersInitialize(jvm);
    if (env == NULL) {
        return -1;
    }

    // BASIC JAVA TESTS
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

    LOG_INFO("Initialization complete");
    return JAVA_VERSION;
}