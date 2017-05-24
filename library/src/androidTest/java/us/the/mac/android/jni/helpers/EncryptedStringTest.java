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

import android.support.test.InstrumentationRegistry;

import org.apache.commons.codec.binary.Base64;
import org.junit.Test;

import java.io.IOException;
import java.util.Scanner;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import static android.R.attr.key;
import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertNotNull;
import static org.junit.Assert.assertNotEquals;
import static us.the.mac.android.jni.helpers.TestConstants.TEST_ENCRYPTED_RESOURCE;

import us.the.mac.android.jni.helpers.EncryptedString;
import us.the.mac.android.jni.helpers.TestConstants;

public class EncryptedStringTest {
    static {
        System.loadLibrary("test-helper-lib");
    }

    native public byte[] getNativeBytes();

    native public SecretKeySpec createSecretKeySpec();

    native public SecretKeySpec createSecretKeySpecWithParams(byte[] decodedKeyBytes, int start, int length, String algorithm);

    native public byte [] nativeBase64EncodeBase64(byte [] inputBytes);

    native public byte [] nativeBase64DecodeBase64(byte [] inputBytes);

    native public CryptoHelper createCryptoHelper();

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
    public void base64Decoding() throws Exception {

        byte[] startingNativeBytes = getNativeBytes();
        byte[] startingBytes = TestConstants.TEST_DECRYPT.getBytes();
        assertEquals(TestConstants.TEST_DECRYPT, new String(startingBytes));
        assertEquals(TestConstants.TEST_DECRYPT, new String(startingNativeBytes));

        byte [] encodedBytes = Base64.encodeBase64( startingBytes );
        byte [] decodedBytes = Base64.decodeBase64( encodedBytes );

        assertEquals(TestConstants.TEST_DECRYPT, new String(decodedBytes));

        byte [] nativeEncodedBytes = nativeBase64EncodeBase64( startingBytes );
        byte [] nativeDecodedBytes = nativeBase64DecodeBase64( nativeEncodedBytes );

        assertEquals(new String(encodedBytes), new String(nativeEncodedBytes));
        assertEquals(new String(decodedBytes), new String(nativeDecodedBytes));
        assertEquals(new String(startingNativeBytes), new String(nativeDecodedBytes));
        assertEquals(TestConstants.TEST_DECRYPT, new String(nativeDecodedBytes));

    }

    @Test
    public void creatingASecretKeySpec() throws Exception {

        byte[] keyBytes = TestConstants.TEST_ENCRYPTION_KEY.getBytes();
        assertEquals(TestConstants.TEST_ENCRYPTION_KEY, new String(keyBytes));

        byte [] decodedKeyBytes = Base64.decodeBase64( keyBytes );
        byte [] encodedKeyBytes = Base64.encodeBase64( decodedKeyBytes );

//        assertEquals(TestConstants.TEST_DECODED_ENCRYPTION_KEY, new String(decodedKeyBytes));

        byte [] nativeDecodedKeyBytes = nativeBase64DecodeBase64( keyBytes );
        byte [] nativeEncodedKeyBytes = nativeBase64EncodeBase64( nativeDecodedKeyBytes );

        assertEquals(new String(encodedKeyBytes), new String(nativeEncodedKeyBytes));
        assertEquals(new String(decodedKeyBytes), new String(nativeDecodedKeyBytes));
//        assertEquals(TestConstants.TEST_DECODED_ENCRYPTION_KEY, new String(nativeDecodedKeyBytes));


        SecretKey originalKey = new SecretKeySpec(decodedKeyBytes, 0, decodedKeyBytes.length, "AES");
        assertEquals(TestConstants.TEST_ENCRYPTION_KEY, new String(Base64.encodeBase64(originalKey.getEncoded())).concat("\n"));
//        assertEquals(TestConstants.TEST_DECODED_ENCRYPTION_KEY, new String(originalKey.getEncoded()).concat("\n"));

        SecretKey nativeKey = createSecretKeySpecWithParams(decodedKeyBytes, 0, decodedKeyBytes.length, "AES");
        assertEquals(originalKey, nativeKey);

    }
    @Test
    public void javaDecryptWithSecretKey() throws Exception {

        String contents = TestConstants.TEST_ENCRYPTED_RESOURCE;
        byte[] keyBytes = TestConstants.TEST_ENCRYPTION_KEY.getBytes();
        String [] parts = contents.split( ":" );

        byte [] decodedKeyBytes = Base64.decodeBase64( keyBytes );
        byte [] initVector = Base64.decodeBase64( parts[0].getBytes() );
        byte [] decodedValueBytes = Base64.decodeBase64(parts[1].getBytes());

        SecretKey originalKey = new SecretKeySpec(decodedKeyBytes, 0, decodedKeyBytes.length, "AES");
        Cipher cipher = Cipher.getInstance(originalKey.getAlgorithm() + "/CBC/PKCS5Padding" );
        cipher.init( Cipher.DECRYPT_MODE, originalKey, new IvParameterSpec( initVector ) );
        byte [] decryptedBytes = cipher.doFinal( decodedValueBytes );

        assertEquals(TestConstants.TEST_DECRYPT, new String(decryptedBytes));

    }

    @Test
    public void nativeDecryptWithSecretKey() throws Exception {

        String contents = TestConstants.TEST_ENCRYPTED_RESOURCE;
        String [] parts = contents.split( ":" );

        byte [] initVector = Base64.decodeBase64( parts[0].getBytes() );
        byte [] decodedValueBytes = Base64.decodeBase64(parts[1].getBytes());

        SecretKey originalKey = createSecretKeySpec();
        Cipher cipher = Cipher.getInstance(originalKey.getAlgorithm() + "/CBC/PKCS5Padding" );
        cipher.init( Cipher.DECRYPT_MODE, originalKey, new IvParameterSpec( initVector ) );
        byte [] decryptedBytes = cipher.doFinal( decodedValueBytes );

        assertEquals(TestConstants.TEST_DECRYPT, new String(decryptedBytes));

    }
    @Test
    public void generateCrytpoHelper() throws Exception {

        CryptoHelper object = createCryptoHelper();
        assertNotEquals(0, object.nPtr);

    }

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

        String encryptedString = InstrumentationRegistry.getTargetContext().getString(R.string.encryptedString);
        assertEquals(encryptedString, object.encryptedString);

        String decryptedString = object.decrypt(EncryptedString.RESOURCE_STRINGS_ALGORITHM);
        assertEquals(TestConstants.TEST_DECRYPT, decryptedString);

        // Should only return the decrypted string field, the encryptedString should remain untouched
        assertEquals(encryptedString, object.encryptedString);
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

    @Test(expected = java.lang.UnsatisfiedLinkError.class)
    public void destroyFromJava() throws Exception {
        EncryptedString object = createEncryptedString();
        assertNotEquals(0, object.nPtr);
        object.destroy();
    }
}
