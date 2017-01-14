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

#ifndef __DecodedString_h__
#define __DecodedString_h__

#include <jni.h>
#include "JniHelpers.h"
#include "../../androidTest/cpp/JniHelpersTest.h"

using namespace spotify::jni;

/**
 * @brief Example of a natively persisted class
 *
 * Instances of this class can be created on the heap and then leaked, the
 * address of the leaked instance is stored in a corresponding Java object
 * in a long field. When you want to access the native instance again, it
 * is provided automatically when invoking ClassRegistry::newInstance.
 *
 * In order for JniHelpers to correctly recognize a class as being persisted,
 * you must call enablePersistence() in initialize().
 */
class DecodedString : public NativeObject {
public:
    DecodedString();

    DecodedString(JNIEnv *env);

    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", DecodedString);
    }

    void initialize(JNIEnv *env);

    void mapFields();

    static jobject getInstance(JNIEnv *env, jobject javaThis);
    static jstring decode(JNIEnv *env, jobject java_this);
    static void nativeDestroy(JNIEnv *env, jobject java_this);


public:
    JavaString encodedString;
};

#endif // __DecodedString_h__
