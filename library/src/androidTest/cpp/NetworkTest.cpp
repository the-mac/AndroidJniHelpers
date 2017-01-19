/*
 * Copyright (c) 2014 Spotify AB
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "NetworkTest.h"
#include "JUnitUtils.h"
#include "Network.h"
#include "JSONObject.h"

using namespace operators::nativity::requests;

void NetworkTest::initialize(JNIEnv *env) {
    setClass(env);

    Network network;
    const char *networkName = network.getCanonicalName();

    addNativeMethod("createNetwork", (void *) &createNetwork, networkName, NULL);
    addNativeMethod("getPersistedInstance", (void *) &getPersistedInstance, networkName,
                    networkName, NULL);
    addNativeMethod("nativeIsPersistenceEnabled", (void *) nativeIsPersistenceEnabled, kTypeVoid,
                    NULL);
    addNativeMethod("isPersistenceEnabledWithoutInit", (void *) isPersistenceEnabledWithoutInit,
                    kTypeVoid, NULL);
    addNativeMethod("destroyNetwork", (void *) &destroyNetwork, kTypeVoid, networkName, NULL);
    addNativeMethod("persistNullObject", (void *) &persistNullObject, kTypeVoid, NULL);
    addNativeMethod("nativeGetPutAndToJSONString", (void *) nativeGetPutAndToJSONString, kTypeVoid,
                    NULL);
    addNativeMethod("nativeNetworkRequest", (void *) nativeNetworkRequest, kTypeVoid, NULL);

    addNativeMethod("destroyInvalidClass", (void *) &destroyInvalidClass, kTypeVoid, NULL);
    addNativeMethod("destroyNullObject", (void *) &destroyNullObject, kTypeVoid, NULL);

    registerNativeMethods(env);
}

jobject NetworkTest::createNetwork(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: createNetwork");
    Network *network = new Network(env);
    network->resultString = TEST_RESULT;
    // Persist should be called for us here. Note that the original object is leaked; it will
    // be cleaned up in destroyNetwork().
    return network->toJavaObject(env);
}

jobject NetworkTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
    LOG_INFO("Starting test: getPersistedInstance");
    ClassRegistry registry;
    registry.add(env, new Network(env));
    Network *network = registry.getNativeInstance<Network>(env, object);
    JUNIT_ASSERT_EQUALS_STRING(TEST_RESULT, network->resultString.get());
    JUNIT_ASSERT_NOT_NULL(network->getCanonicalName());
    JUNIT_ASSERT_TRUE(network->isInitialized());
    return network->toJavaObject(env);
}

void NetworkTest::nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeIsPersistenceEnabled");
    Network network(env);
    JUNIT_ASSERT_TRUE(network.isInitialized());
    Network mergedObject;
    mergedObject.merge(&network);
    JUNIT_ASSERT_TRUE(mergedObject.isInitialized());
    jstring key = env->NewStringUTF("parameter");
}

void NetworkTest::isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis) {
    Network network;
    JUNIT_ASSERT_FALSE(network.isInitialized());
}

void NetworkTest::destroyNetwork(JNIEnv *env, jobject javaThis, jobject object) {
    LOG_INFO("Starting test: destroyNetwork");
    ClassRegistry registry;
    registry.add(env, new Network(env));
    Network *network = registry.getNativeInstance<Network>(env, object);
    network->destroy(env, object);
}

void NetworkTest::persistNullObject(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: persistNullObject");
    Network network(env);
    network.mapFields();
    JUNIT_ASSERT_FALSE(network.persist(env, NULL));
}

void NetworkTest::nativeGetPutAndToJSONString(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeGetPutAndToJSONString");

    Network *network = new Network(env);
    jobject networkObj = network->toJavaObject(env);

    // "parameter", "parameterValue"
    jstring key = env->NewStringUTF("parameter");
    jstring value = env->NewStringUTF("parameterValue");
    network->put(env, networkObj, key, value);

    jstring output = network->get(env, networkObj, key);
    std::string testString = env->GetStringUTFChars(output, 0);
    JUNIT_ASSERT_EQUALS_STRING(TEST_PARAMETER, testString);

    std::string jsonOutput = network->toJSON();
    JUNIT_ASSERT_EQUALS_STRING(TEST_REQUEST, jsonOutput);
}

void NetworkTest::nativeNetworkRequest(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeNetworkRequest");

    Network *network = new Network(env);
    jobject networkObj = network->toJavaObject(env);
    std::string value =  network->resultString.get();
    JUNIT_ASSERT_EQUALS_STRING("", value);


    jstring response =  network->request(env, Network::HTTP_BIN);
    JavaString resultString = JavaString(env->GetStringUTFChars(response, 0));

    std::string result =  resultString.get();
    JUNIT_ASSERT_NOT_EQUALS_STRING("", result);
    LOG_INFO(network->getCanonicalName(), "The result string is %s", resultString.get());

    int length = resultString.get().length();
    LOG_INFO(network->getCanonicalName(), "The result string length is %d", length);
    JUNIT_ASSERT_TRUE(length > 0);

    JSONObject jsonObject(env, resultString.get());
    JavaString requestUrl = jsonObject.getString(env, "url");
    JUNIT_ASSERT_EQUALS_STRING(TEST_URL, requestUrl.get());

    JSONObject headers = jsonObject.getJSONObject(env, "headers");

    JavaString acceptString = headers.getString(env, "Accept");
    JUNIT_ASSERT_EQUALS_STRING(TEST_HEADERS, acceptString.get());

    JavaString contentString = headers.getString(env, "Content-Type");
    JUNIT_ASSERT_EQUALS_STRING(TEST_HEADERS, contentString.get());

}

void NetworkTest::destroyInvalidClass(JNIEnv *env, jobject javaThis) {
    // This test is almost impossible to replicate from Java, and frankly should
    // not happen from (responsible) C++ code either. It would be possible to catch
    // if we are willing to do fieldID lookups on the fly rather than cached, but
    // that assumes that performance is not an issue here. For that reason, this
    // test is excluded and the erroneous behavior will (and probably should) crash
    // the JVM if enabled.
#if 0
    LOG_INFO("Starting test: destroyInvalidClass");
    Network network(env);
    network.mapFields();
    network.destroy(env, javaThis);
#endif
}

void NetworkTest::destroyNullObject(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: destroyNullObject");
    Network network(env);
    network.mapFields();
    network.destroy(env, NULL);
}
