#include <jni.h>
#include <string>
#include <JniHelpers.h>
#include <JniHelpersTest.h>
#include <JavaStringTest.h>
#include <NativeObjectTest.h>
#include <PersistedObject.h>
#include <DecodedStringTest.h>
#include <DecodedString.h>

using namespace spotify::jni;
using namespace operators::nativity::decodedstring;
ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
    LOG_INFO("Initializing JNI");
    JNIEnv *env = jniHelpersInitialize(jvm);
    if (env == NULL) {
        return -1;
    }

    gClasses.add(env, new JavaStringTest(env));
    gClasses.add(env, new NativeObjectTest(env));
    gClasses.add(env, new PersistedObject(env));
    gClasses.add(env, new DecodedStringTest(env));
    gClasses.add(env, new DecodedString(env));

    LOG_INFO("Initialization complete");
    return JAVA_VERSION;
}