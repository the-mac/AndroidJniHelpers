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

#ifndef __NetworkTest_h__
#define __NetworkTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

namespace operators {
    namespace nativity {
        namespace requests {

class NetworkTest : public JavaClass {
public:
  NetworkTest() : JavaClass() {}
  NetworkTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~NetworkTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME("com/operators/nativity/requests", NetworkTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}

private:
  static jobject createNetwork(JNIEnv *env, jobject javaThis);
  static jobject getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object);
  static void nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis);
  static void isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis);
  static void destroyNetwork(JNIEnv *env, jobject javaThis, jobject object);
  static void persistNullObject(JNIEnv *env, jobject javaThis);
  static void nativeGetPutAndToJSONString(JNIEnv *env, jobject javaThis);
  static void nativeNetworkRequest(JNIEnv *env, jobject javaThis);
  static void destroyInvalidClass(JNIEnv *env, jobject javaThis);
  static void destroyNullObject(JNIEnv *env, jobject javaThis);
};

} // namespace decodedstring
} // namespace nativity
} // namespace operators

#endif // __NetworkTest_h__
