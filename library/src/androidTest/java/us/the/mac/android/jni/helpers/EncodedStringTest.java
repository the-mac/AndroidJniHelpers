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

package us.the.mac.android.jni.helpers;

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

public class EncodedStringTest {
    static {
        System.loadLibrary("test-helper-lib");
    }

    native public EncodedString createEncodedString();

    native public EncodedString getPersistedInstance(EncodedString object);

    native public void destroyEncodedString(EncodedString object);

    @Test
    public void persist() throws Exception {
        EncodedString object = createEncodedString();
        assertNotEquals(0, object.nPtr);
        // These properties should be set by the first native method in this case
        assertEquals(TestConstants.TEST_ENCODE, object.encodedString);

        // Now create a new empty object, but copy the nPtr field to it. Note that
        // the i field is *not* copied; that value is stored natively and should
        // be retrieved in the call to getPersistedInstance.
        EncodedString emptyInstance = EncodedString.testingInstance();
        emptyInstance.nPtr = object.nPtr;

        // The native test should be able to fetch the previous instance via nPtr,
        // and return to us the same instance data in a new object.
        EncodedString result = getPersistedInstance(emptyInstance);
        assertEquals(object.nPtr, result.nPtr);
        assertEquals(object.encodedString, result.encodedString);

        // Always clean up after yourself, kids!
        destroyEncodedString(object);
    }

    @Test(expected = IllegalArgumentException.class)
    native public void persistNullObject() throws Exception;

    @Test
    public void decodeString() throws Exception {
        EncodedString object = createEncodedString();
        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_ENCODE, object.encodedString);

        String decodedString = object.decode();
        assertEquals(TestConstants.TEST_DECODE, decodedString);

        // Decode should only return the decoded string field, the encodedString should remain untouched
        assertEquals(TestConstants.TEST_ENCODE, object.encodedString);
    }

    @Test
    public void destroyEncodedString() throws Exception {
        EncodedString object = createEncodedString();
        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_ENCODE, object.encodedString);

        destroyEncodedString(object);

        assertEquals(0, object.nPtr);
        // Destroy should only alter the nPtr field, this should remain untouched
        assertEquals(TestConstants.TEST_ENCODE, object.encodedString);
    }

    @Test
    native public void nativeIsPersistenceEnabled() throws Exception;

    @Test
    native public void isPersistenceEnabledWithoutInit() throws Exception;

    @Test
    native public void destroyInvalidClass() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void destroyNullObject() throws Exception;

    @Test
    public void destroyFromJava() throws Exception {
        EncodedString object = createEncodedString();
        assertNotEquals(0, object.nPtr);
        object.destroy();
    }
}
