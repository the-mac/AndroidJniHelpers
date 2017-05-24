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

#include "CryptoTest.h"
#include "JUnitUtils.h"
#include "CryptoHelper.h"
#include "JSONObject.h"

void CryptoTest::initialize(JNIEnv *env) {

    CryptoHelper helper;
    const char *helperName = helper.getCanonicalName();

    addNativeMethod("createCryptoHelper", (void *) &createCryptoHelper, helperName, NULL);
    addNativeMethod("getPersistedInstance", (void *) &getPersistedInstance, helperName,
                    helperName, NULL);
    addNativeMethod("nativeIsPersistenceEnabled", (void *) nativeIsPersistenceEnabled, kTypeVoid,
                    NULL);
    addNativeMethod("isPersistenceEnabledWithoutInit", (void *) isPersistenceEnabledWithoutInit,
                    kTypeVoid, NULL);
    addNativeMethod("destroyCryptoHelper", (void *) &destroyCryptoHelper, kTypeVoid, helperName, NULL);
    addNativeMethod("persistNullObject", (void *) &persistNullObject, kTypeVoid, NULL);
    addNativeMethod("nativeGetPutAndToJSONString", (void *) nativeGetPutAndToJSONString, kTypeVoid,
                    NULL);
    addNativeMethod("nativeCryptoHelperRequest", (void *) nativeCryptoHelperRequest, kTypeVoid, NULL);

    addNativeMethod("destroyInvalidClass", (void *) &destroyInvalidClass, kTypeVoid, NULL);
    addNativeMethod("destroyNullObject", (void *) &destroyNullObject, kTypeVoid, NULL);

    registerNativeMethods(env);
}

jobject CryptoTest::createCryptoHelper(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: createCryptoHelper");
    CryptoHelper *helper = new CryptoHelper(env);
//    helper->resultString = TEST_RESULT;
    // Persist should be called for us here. Note that the original object is leaked; it will
    // be cleaned up in destroyCryptoHelper().
    return helper->toJavaObject(env);
}

jobject CryptoTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
    LOG_INFO("Starting test: getPersistedInstance");
    ClassRegistry registry;
    registry.add(env, new CryptoHelper(env));
    CryptoHelper *helper = registry.getNativeInstance<CryptoHelper>(env, object);
//    JUNIT_ASSERT_EQUALS_STRING(TEST_RESULT, helper->resultString.get());
    JUNIT_ASSERT_NOT_NULL(helper->getCanonicalName());
    JUNIT_ASSERT_TRUE(helper->isInitialized());
    return helper->toJavaObject(env);
}

void CryptoTest::nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeIsPersistenceEnabled");
    CryptoHelper helper(env);
    JUNIT_ASSERT_TRUE(helper.isInitialized());
    CryptoHelper mergedObject;
    mergedObject.merge(&helper);
    JUNIT_ASSERT_TRUE(mergedObject.isInitialized());
    jstring key = env->NewStringUTF("parameter");
}

void CryptoTest::isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis) {
    CryptoHelper helper;
    JUNIT_ASSERT_FALSE(helper.isInitialized());
}

void CryptoTest::destroyCryptoHelper(JNIEnv *env, jobject javaThis, jobject object) {
    LOG_INFO("Starting test: destroyCryptoHelper");
    ClassRegistry registry;
    registry.add(env, new CryptoHelper(env));
    CryptoHelper *helper = registry.getNativeInstance<CryptoHelper>(env, object);
    helper->destroy(env, object);
}

void CryptoTest::persistNullObject(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: persistNullObject");
    CryptoHelper helper(env);
    helper.mapFields();
    JUNIT_ASSERT_FALSE(helper.persist(env, NULL));
}

void CryptoTest::nativeGetPutAndToJSONString(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeGetPutAndToJSONString");

    CryptoHelper *helper = new CryptoHelper(env);

    // "parameter", "parameterValue"
//    jstring key = env->NewStringUTF("parameter");
//    jstring value = env->NewStringUTF("parameterValue");
//    helper->putNative(env, key, value);
//
//    jstring output = helper->getNative(env, key);
//    std::string testString = env->GetStringUTFChars(output, 0);
//    JUNIT_ASSERT_EQUALS_STRING(TEST_PARAMETER, testString);
//
//    std::string jsonOutput = helper->requestJSON();
//    JUNIT_ASSERT_EQUALS_STRING(TEST_REQUEST, jsonOutput);
}

void CryptoTest::nativeCryptoHelperRequest(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: nativeCryptoHelperRequest");

    CryptoHelper *helper = new CryptoHelper(env);
//    std::string value =  helper->resultString.get();
//    JUNIT_ASSERT_EQUALS_STRING("", value);
//
//
//    jstring response = helper->request(env, CryptoHelper::HTTP_BIN);
//    JUNIT_ASSERT_NOT_NULL(response);

//    JavaString resultString(env, response);
//    std::string result =  resultString.get();
//    JUNIT_ASSERT_NOT_EQUALS_STRING("", result);
//    LOG_INFO(helper->getCanonicalName(), "The result string is %s", resultString.get());
//
//    int length = resultString.get().length();
//    LOG_INFO(helper->getCanonicalName(), "The result string length is %d", length);
//    JUNIT_ASSERT_TRUE(length > 0);
//
//    JSONObject jsonObject(env, resultString.get());
//    JavaString requestUrl(env, jsonObject.getString(env, "url"));
//    JUNIT_ASSERT_EQUALS_STRING(TEST_URL, requestUrl.get());
//
//    JSONObject headers(env, jsonObject.getJSONObject(env, "headers"));
//
//    JavaString acceptString(env, headers.getString(env, "Accept"));
//    JUNIT_ASSERT_EQUALS_STRING(TEST_HEADERS, acceptString.get());
//
//    JavaString contentString(env, headers.getString(env, "Content-Type"));
//    JUNIT_ASSERT_EQUALS_STRING(TEST_HEADERS, contentString.get());

}

void CryptoTest::destroyInvalidClass(JNIEnv *env, jobject javaThis) {
    // This test is almost impossible to replicate from Java, and frankly should
    // not happen from (responsible) C++ code either. It would be possible to catch
    // if we are willing to do fieldID lookups on the fly rather than cached, but
    // that assumes that performance is not an issue here. For that reason, this
    // test is excluded and the erroneous behavior will (and probably should) crash
    // the JVM if enabled.
#if 0
    LOG_INFO("Starting test: destroyInvalidClass");
    CryptoHelper helper(env);
    helper.mapFields();
    helper.destroy(env, javaThis);
#endif
}

void CryptoTest::destroyNullObject(JNIEnv *env, jobject javaThis) {
    LOG_INFO("Starting test: destroyNullObject");
    CryptoHelper helper(env);
    helper.mapFields();
    helper.destroy(env, NULL);
}
