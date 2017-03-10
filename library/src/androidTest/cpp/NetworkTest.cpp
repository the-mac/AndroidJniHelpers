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
#include "MACRequests.h"
#include "JSONObject.h"

using namespace operators::nativity::requests;

void NetworkTest::initialize(JNIEnv *env) {

    MACRequests network;
    const char *networkName = network.getCanonicalName();

    addNativeMethod("createMACRequests", (void *) &createMACRequests, networkName, NULL);
    addNativeMethod("getPersistedInstance", (void *) &getPersistedInstance, networkName,
                    networkName, NULL);
    addNativeMethod("nativeIsPersistenceEnabled", (void *) nativeIsPersistenceEnabled, kTypeVoid,
                    NULL);
    addNativeMethod("isPersistenceEnabledWithoutInit", (void *) isPersistenceEnabledWithoutInit,
                    kTypeVoid, NULL);
    addNativeMethod("destroyMACRequests", (void *) &destroyMACRequests, kTypeVoid, networkName, NULL);
    addNativeMethod("persistNullObject", (void *) &persistNullObject, kTypeVoid, NULL);
    addNativeMethod("nativeGetPutAndToJSONString", (void *) nativeGetPutAndToJSONString, kTypeVoid,
                    NULL);
    addNativeMethod("nativeMACRequestsRequest", (void *) nativeMACRequestsRequest, kTypeVoid, NULL);

    addNativeMethod("destroyInvalidClass", (void *) &destroyInvalidClass, kTypeVoid, NULL);
    addNativeMethod("destroyNullObject", (void *) &destroyNullObject, kTypeVoid, NULL);

    registerNativeMethods(env);
}

jobject NetworkTest::createMACRequests(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: createMACRequests");
    MACRequests *network = new MACRequests(env);
    network->resultString = TEST_RESULT;
    // Persist should be called for us here. Note that the original object is leaked; it will
    // be cleaned up in destroyMACRequests().
    return network->toJavaObject(env);
}

jobject NetworkTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
    LOG_INFO("Starting test: getPersistedInstance");
    ClassRegistry registry;
    registry.add(env, new MACRequests(env));
    MACRequests *network = registry.getNativeInstance<MACRequests>(env, object);
    JUNIT_ASSERT_EQUALS_STRING(TEST_RESULT, network->resultString.get());
    JUNIT_ASSERT_NOT_NULL(network->getCanonicalName());
    JUNIT_ASSERT_TRUE(network->isInitialized());
    return network->toJavaObject(env);
}

void NetworkTest::nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeIsPersistenceEnabled");
    MACRequests network(env);
    JUNIT_ASSERT_TRUE(network.isInitialized());
    MACRequests mergedObject;
    mergedObject.merge(&network);
    JUNIT_ASSERT_TRUE(mergedObject.isInitialized());
    jstring key = env->NewStringUTF("parameter");
}

void NetworkTest::isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis) {
    MACRequests network;
    JUNIT_ASSERT_FALSE(network.isInitialized());
}

void NetworkTest::destroyMACRequests(JNIEnv *env, jobject javaThis, jobject object) {
    LOG_INFO("Starting test: destroyMACRequests");
    ClassRegistry registry;
    registry.add(env, new MACRequests(env));
    MACRequests *network = registry.getNativeInstance<MACRequests>(env, object);
    network->destroy(env, object);
}

void NetworkTest::persistNullObject(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: persistNullObject");
    MACRequests network(env);
    network.mapFields();
    JUNIT_ASSERT_FALSE(network.persist(env, NULL));
}

void NetworkTest::nativeGetPutAndToJSONString(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeGetPutAndToJSONString");

    MACRequests *network = new MACRequests(env);

    // "parameter", "parameterValue"
    jstring key = env->NewStringUTF("parameter");
    jstring value = env->NewStringUTF("parameterValue");
    network->putNative(env, key, value);

    jstring output = network->getNative(env, key);
    std::string testString = env->GetStringUTFChars(output, 0);
    JUNIT_ASSERT_EQUALS_STRING(TEST_PARAMETER, testString);

    std::string jsonOutput = network->requestJSON();
    JUNIT_ASSERT_EQUALS_STRING(TEST_REQUEST, jsonOutput);
}

void NetworkTest::nativeMACRequestsRequest(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeMACRequestsRequest");

    MACRequests *network = new MACRequests(env);
    std::string value =  network->resultString.get();
    JUNIT_ASSERT_EQUALS_STRING("", value);


    jstring response = network->request(env, MACRequests::HTTP_BIN);
    JUNIT_ASSERT_NOT_NULL(response);

    JavaString resultString(env, response);
    std::string result =  resultString.get();
    JUNIT_ASSERT_NOT_EQUALS_STRING("", result);
    LOG_INFO(network->getCanonicalName(), "The result string is %s", resultString.get());

    int length = resultString.get().length();
    LOG_INFO(network->getCanonicalName(), "The result string length is %d", length);
    JUNIT_ASSERT_TRUE(length > 0);

    JSONObject jsonObject(env, resultString.get());
    JavaString requestUrl(env, jsonObject.getString(env, "url"));
    JUNIT_ASSERT_EQUALS_STRING(TEST_URL, requestUrl.get());

    JSONObject headers(env, jsonObject.getJSONObject(env, "headers"));

    JavaString acceptString(env, headers.getString(env, "Accept"));
    JUNIT_ASSERT_EQUALS_STRING(TEST_HEADERS, acceptString.get());

    JavaString contentString(env, headers.getString(env, "Content-Type"));
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
    MACRequests network(env);
    network.mapFields();
    network.destroy(env, javaThis);
#endif
}

void NetworkTest::destroyNullObject(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: destroyNullObject");
    MACRequests network(env);
    network.mapFields();
    network.destroy(env, NULL);
}
