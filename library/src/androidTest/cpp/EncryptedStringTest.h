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

#ifndef __EncryptedStringTest_h__
#define __EncryptedStringTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;


class EncryptedStringTest : public JavaClass {
public:
  EncryptedStringTest() : JavaClass() {}
  EncryptedStringTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~EncryptedStringTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, EncryptedStringTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}

private:
  static jbyteArray getNativeBytes(JNIEnv *env, jobject javaThis);
  static jbyteArray nativeBase64EncodeBase64(JNIEnv *env, jobject javaThis, jbyteArray inputBytes);
  static jbyteArray nativeBase64DecodeBase64(JNIEnv *env, jobject javaThis, jbyteArray inputBytes);
  static jobject createSecretKeySpecWithParams(JNIEnv *env, jobject javaThis,
                                               jbyteArray decodedKeyBytes, jint start, jint length,
                                               jstring algorithm);
  static jobject createSecretKeySpec(JNIEnv *env, jobject javaThis);

  static jobject createCryptoHelper(JNIEnv *env, jobject javaThis);
  static jobject createEncryptedString(JNIEnv *env, jobject javaThis);
  static jobject createEncryptedResourceString(JNIEnv *env, jobject javaThis);
  static jobject getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object);
  static void nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis);
  static void isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis);
  static void destroyEncryptedString(JNIEnv *env, jobject javaThis, jobject object);
  static void persistNullObject(JNIEnv *env, jobject javaThis);
  static void destroyInvalidClass(JNIEnv *env, jobject javaThis);
  static void destroyNullObject(JNIEnv *env, jobject javaThis);
};


#endif // __EncryptedStringTest_h__
