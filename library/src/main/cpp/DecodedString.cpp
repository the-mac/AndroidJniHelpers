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

#include <string>
#include <jni.h>
#include <iostream>
#include <sstream>
#include "DecodedString.h"

using namespace std;

DecodedString::DecodedString() : NativeObject() {}

DecodedString::DecodedString(JNIEnv *env) : NativeObject(env) {//}
  initialize(env);
}

void DecodedString::initialize(JNIEnv *env) {
  LOG_INFO("\nCalling DecodedString: " __func__);
  setClass(env);
  cacheConstructor(env);
  cacheField(env, "encodedString", kTypeString);
#if ANDROID
  addNativeMethod("getInstance", (void*)&DecodedString::getInstance, getCanonicalName(), NULL);
  addNativeMethod("decode", (void*)&DecodedString::decode, kTypeString, NULL);
  addNativeMethod("destroy", (void*)&DecodedString::nativeDestroy, kTypeVoid, NULL);
#else
  addNativeMethod("decode", (void*)&DecodedString::decode, kTypeString, NULL);
  addNativeMethod("destroy", (void*)&DecodedString::nativeDestroy, kTypeVoid, NULL);
#endif
  registerNativeMethods(env);
}

void DecodedString::mapFields() {
  mapField("encodedString", kTypeString, &encodedString);
}

string rot(string s, int r)
{
//    string nonAlphas = "";
//    string nonVisuals = "";

  stringstream ss;
  for (int i = 0; i < s.length(); i++)
  {
    char c = s[i];
    if (c >= 'a' && c <= 'z') ss << (char)((c - 'a' + r) % 26 + 'a');
    else if (c >= 'A' && c <= 'Z') ss << (char)((c - 'A' + r) % 26 + 'A');
    else if (c >= '{' && c <= '~') ss << (char)((c - 'a' + r) % 26 + 'a');
    else if (c >= 'A' && c <= 'Z') ss << (char)((c - 'A' + r) % 26 + 'A');
    else ss << c;
  }
  return ss.str();
}

jobject DecodedString::getInstance(JNIEnv *env, jobject javaThis) {
  DecodedString *decodedString = new DecodedString(env);
  return decodedString->toJavaObject(env);
}
jstring DecodedString::decode(JNIEnv *env, jobject java_this) {
    DecodedString *object = gClasses.getNativeInstance<DecodedString>(env, java_this);

    if (object != NULL) {

    string conversion = rot(object->encodedString.get(), -1);
    jstring decodedString = env->NewStringUTF(conversion.c_str());

    return decodedString;

  }

  return NULL;
}
jstring DecodedString::encode(JNIEnv *env) {
    string conversion = rot(encodedString.get(), -1);
    jstring decodedString = env->NewStringUTF(conversion.c_str());

    return decodedString;
}
void DecodedString::nativeDestroy(JNIEnv *env, jobject java_this) {
  DecodedString *object = gClasses.getNativeInstance<DecodedString>(env, java_this);
  if (object != NULL) {
    // NativeObject *native_object = dynamic_cast<NativeObject*>(object);
    object->destroy(env, java_this);
  }
}
