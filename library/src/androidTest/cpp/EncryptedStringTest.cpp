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

#include "EncryptedStringTest.h"
#include "JUnitUtils.h"
#include "EncryptedString.h"
#include "TestObject.h"

void EncryptedStringTest::initialize(JNIEnv *env) {
  setClass(env);

  EncryptedString encryptedString;
  const char* encryptedStringName = encryptedString.getCanonicalName();

  CryptoHelper cryptoHelper;
  const char* cryptoHelperName = cryptoHelper.getCanonicalName();

  addNativeMethod("createCryptoHelper", (void*)&createCryptoHelper, cryptoHelperName, NULL);
  addNativeMethod("createEncryptedString", (void*)&createEncryptedString, encryptedStringName, NULL);
  addNativeMethod("createEncryptedResourceString", (void*)&createEncryptedResourceString, encryptedStringName, NULL);
  addNativeMethod("getPersistedInstance", (void*)&getPersistedInstance, encryptedStringName, encryptedStringName, NULL);

  addNativeMethod("nativeIsPersistenceEnabled", (void*)nativeIsPersistenceEnabled, kTypeVoid, NULL);
  addNativeMethod("isPersistenceEnabledWithoutInit", (void*)isPersistenceEnabledWithoutInit, kTypeVoid, NULL);
  addNativeMethod("destroyEncryptedString", (void*)&destroyEncryptedString, kTypeVoid, encryptedStringName, NULL);
  addNativeMethod("persistNullObject", (void*)&persistNullObject, kTypeVoid, NULL);
  addNativeMethod("destroyInvalidClass", (void*)&destroyInvalidClass, kTypeVoid, NULL);
  addNativeMethod("destroyNullObject", (void*)&destroyNullObject, kTypeVoid, NULL);

  registerNativeMethods(env);
}

jobject EncryptedStringTest::createCryptoHelper(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: createCryptoHelper");
  CryptoHelper *cryptoHelper = new CryptoHelper(env);
  return cryptoHelper->toJavaObject(env);
}


jobject EncryptedStringTest::createEncryptedString(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: createEncryptedString");
  EncryptedString *encryptedString = new EncryptedString(env);
  encryptedString->encryptedString = TEST_ENCRYPTED;
  // Persist should be called for us here. Note that the original object is leaked; it will
  // be cleaned up in destroyEncryptedString().
  return encryptedString->toJavaObject(env);
}

jobject EncryptedStringTest::createEncryptedResourceString(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: createEncryptedString");
  EncryptedString *encryptedString = new EncryptedString(env);
  encryptedString->encryptedString = TEST_ENCRYPTED_RESOURCE;
  // Persist should be called for us here. Note that the original object is leaked; it will
  // be cleaned up in destroyEncryptedString().
  return encryptedString->toJavaObject(env);
}

jobject EncryptedStringTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
  LOG_INFO("Starting test: getPersistedInstance");
  ClassRegistry registry;
  registry.add(env, new EncryptedString(env));
  EncryptedString *encryptedString = registry.getNativeInstance<EncryptedString>(env, object);
  JUNIT_ASSERT_EQUALS_STRING(TEST_ENCRYPTED, encryptedString->encryptedString.get());
  JUNIT_ASSERT_NOT_NULL(encryptedString->getCanonicalName());
  JUNIT_ASSERT_TRUE(encryptedString->isInitialized());
  return encryptedString->toJavaObject(env);
}

void EncryptedStringTest::nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: nativeIsPersistenceEnabled");
  EncryptedString encryptedString(env);
  JUNIT_ASSERT_TRUE(encryptedString.isInitialized());
  EncryptedString mergedObject;
  mergedObject.merge(&encryptedString);
  JUNIT_ASSERT_TRUE(mergedObject.isInitialized());
}

void EncryptedStringTest::isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis) {
  EncryptedString encryptedString;
  JUNIT_ASSERT_FALSE(encryptedString.isInitialized());
}

void EncryptedStringTest::destroyEncryptedString(JNIEnv *env, jobject javaThis, jobject object) {
  LOG_INFO("Starting test: destroyEncryptedString");
  ClassRegistry registry;
  registry.add(env, new EncryptedString(env));
  EncryptedString *encryptedString = registry.getNativeInstance<EncryptedString>(env, object);
  encryptedString->destroy(env, object);
}

void EncryptedStringTest::persistNullObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: persistNullObject");
  EncryptedString encryptedString(env);
  encryptedString.mapFields();
  JUNIT_ASSERT_FALSE(encryptedString.persist(env, NULL));
}

void EncryptedStringTest::destroyInvalidClass(JNIEnv *env, jobject javaThis) {
  // This test is almost impossible to replicate from Java, and frankly should
  // not happen from (responsible) C++ code either. It would be possible to catch
  // if we are willing to do fieldID lookups on the fly rather than cached, but
  // that assumes that performance is not an issue here. For that reason, this
  // test is excluded and the erroneous behavior will (and probably should) crash
  // the JVM if enabled.
#if 0
  LOG_INFO("Starting test: destroyInvalidClass");
  EncryptedString encryptedString(env);
  encryptedString.mapFields();
  encryptedString.destroy(env, javaThis);
#endif
}

void EncryptedStringTest::destroyNullObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: destroyNullObject");
  EncryptedString encryptedString(env);
  encryptedString.mapFields();
  encryptedString.destroy(env, NULL);
}
