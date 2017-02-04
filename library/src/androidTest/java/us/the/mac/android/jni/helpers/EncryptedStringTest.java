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
import static us.the.mac.android.jni.helpers.EncryptedString.RESOURCE_STRINGS_ALGORITHM;
import static us.the.mac.android.jni.helpers.TestConstants.TEST_ROT1_RESOURCE;

public class EncryptedStringTest {
    static {
        System.loadLibrary("test-helper-lib");
    }

    native public EncryptedString createEncryptedString();

    native public EncryptedString createEncryptedResourceString();

    native public EncryptedString getPersistedInstance(EncryptedString object);

    native public void destroyEncryptedString(EncryptedString object);

    @Test
    public void persist() throws Exception {
        EncryptedString object = createEncryptedString();
        assertNotEquals(0, object.nPtr);
        // These properties should be set by the first native method in this case
        assertEquals(TestConstants.TEST_ENCRYPTED, object.encryptedString);

        // Now create a new empty object, but copy the nPtr field to it. Note that
        // the i field is *not* copied; that value is stored natively and should
        // be retrieved in the call to getPersistedInstance.
        EncryptedString emptyInstance = EncryptedString.testingInstance();
        emptyInstance.nPtr = object.nPtr;

        // The native test should be able to fetch the previous instance via nPtr,
        // and return to us the same instance data in a new object.
        EncryptedString result = getPersistedInstance(emptyInstance);
        assertEquals(object.nPtr, result.nPtr);
        assertEquals(object.encryptedString, result.encryptedString);

        // Always clean up after yourself, kids!
        destroyEncryptedString(object);
    }

    @Test(expected = IllegalArgumentException.class)
    native public void persistNullObject() throws Exception;

    @Test
    public void decryptString() throws Exception {
        EncryptedString object = createEncryptedString();
        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_ENCRYPTED, object.encryptedString);

        String decryptedString = object.decrypt(EncryptedString.INLINE_STRINGS_ALGORITHM);
        assertEquals(TestConstants.TEST_DECRYPT, decryptedString);

        // Should only return the decrypted string field, the encryptedString should remain untouched
        assertEquals(TestConstants.TEST_ENCRYPTED, object.encryptedString);

    }

    @Test
    public void decryptResourceString() throws Exception {
        EncryptedString object = createEncryptedResourceString();
        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_ENCRYPTED_RESOURCE, object.encryptedString);

        String decryptedString = object.decrypt(EncryptedString.RESOURCE_STRINGS_ALGORITHM);
        assertEquals(TestConstants.TEST_DECRYPT, decryptedString);

        // Should only return the decrypted string field, the encryptedString should remain untouched
        assertEquals(TestConstants.TEST_ENCRYPTED_RESOURCE, object.encryptedString);

    }

    @Test
    public void destroyEncryptedString() throws Exception {
        EncryptedString object = createEncryptedString();
        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_ENCRYPTED, object.encryptedString);

        destroyEncryptedString(object);

        assertEquals(0, object.nPtr);
        // Destroy should only alter the nPtr field, this should remain untouched
        assertEquals(TestConstants.TEST_ENCRYPTED, object.encryptedString);
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
        EncryptedString object = createEncryptedString();
        assertNotEquals(0, object.nPtr);
        object.destroy();
    }
}
