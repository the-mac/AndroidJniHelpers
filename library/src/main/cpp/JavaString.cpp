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

#include <cstring>
#include "JavaString.h"
#include "JavaExceptionUtils.h"
#include "ByteArray.h"

namespace spotify {
    namespace jni {

        JavaString::JavaString() {
            _value = "";
        }

        JavaString::JavaString(const const char* &cstring) {
            _value = cstring;
        }

        JavaString::JavaString(const std::string &string) {
            _value = string;
        }

        JavaString::JavaString(JNIEnv *env, jstring javaString) {
            set(env, javaString);
        }

        JavaString::JavaString(JNIEnv *env, jbyteArray javaBytes) {
            char *data = (char *) env->GetByteArrayElements(javaBytes, JNI_FALSE);
            set(env, env->NewStringUTF(data));
        }

        const std::string& JavaString::get() const {
            return _value;
        }

        JniLocalRef<jstring> JavaString::toJavaString(JNIEnv *env) const {
            return env->NewStringUTF(_value.c_str());
        }

        JniLocalRef<jbyteArray> JavaString::toByteArray(JNIEnv *env) const {

            const char *content = _value.c_str();
            jbyteArray nativeBytes = env->NewByteArray(_value.length());

            int size = std::strlen(content);

            jbyte *data = (jbyte *) content;
            env->SetByteArrayRegion(nativeBytes, 0, size, data);

            return nativeBytes;
        }

        void JavaString::set(const char *value) {
            _value = value;
        }

        void JavaString::set(const std::string &value) {
            _value = value;
        }

        void JavaString::set(JNIEnv *env, jstring javaString) {
            if (javaString == NULL) {
                return;
            }

            const char *string = env->GetStringUTFChars(javaString, 0);
            JavaExceptionUtils::checkException(env);
            if (string == NULL) {
                return;
            }

            _value = string;
            env->ReleaseStringUTFChars(javaString, string);
            JavaExceptionUtils::checkException(env);
        }

        void JavaString::concat(const char *stringValue) {
            _value += stringValue;
        }

        int JavaString::length() {
            return _value.length();
        }


    } // namespace jni
} // namespace spotify
