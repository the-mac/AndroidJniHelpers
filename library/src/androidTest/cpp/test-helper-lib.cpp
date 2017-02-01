#include <jni.h>
#include <string>
#include <JniHelpers.h>
#include "JniHelpersTest.h"
#include "JavaStringTest.h"
#include "NativeObjectTest.h"
#include "PersistedObject.h"
#include "EncodedStringTest.h"
#include "JavaClassTest.h"
#include "TestObject.h"
#include <DecodedString.h>
#include <MACRequests.h>
#include "NetworkTest.h"
#include "Network.h"

using namespace spotify::jni;
using namespace operators::nativity::decodedstring;
using namespace operators::nativity::requests;

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
    gClasses.add(env, new EncodedStringTest(env));
    gClasses.add(env, new DecodedString(env));
    gClasses.add(env, new JavaClassTest(env));
    gClasses.add(env, new TestObject(env));
    gClasses.add(env, new NetworkTest(env));
    gClasses.add(env, new Network(env));
    gClasses.add(env, new MACRequests(env));

    LOG_INFO("Initialization complete");
    return JAVA_VERSION;
}