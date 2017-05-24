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

import org.apache.http.Header;
import org.apache.http.client.methods.HttpPost;
import org.json.JSONObject;
import org.junit.Test;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.Scanner;

import static junit.framework.Assert.assertEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

public class NetworkTest {
    static {
        System.loadLibrary("test-helper-lib");
    }

    native public MACRequests createMACRequests();

    native public MACRequests getPersistedInstance(MACRequests object);

    native public void destroyMACRequests(MACRequests object);

    @Test
    public void persist() throws Exception {
        MACRequests object = createMACRequests();
        assertNotEquals(0, object.nPtr);
        // These properties should be set by the first native method in this case
        assertEquals(null, object.resultString);

        // Now create a new empty object, but copy the nPtr field to it. Note that
        // the i field is *not* copied; that value is stored natively and should
        // be retrieved in the call to getPersistedInstance.
        MACRequests emptyInstance = MACRequests.testingDefault();
        emptyInstance.nPtr = object.nPtr;

        // The native test should be able to fetch the previous instance via nPtr,
        // and return to us the same instance data in a new object.
        MACRequests result = getPersistedInstance(emptyInstance);
        org.junit.Assert.assertEquals(object.nPtr, result.nPtr);
        assertEquals(object.resultString, result.resultString);

        // Always clean up after yourself, kids!
        destroyMACRequests(object);
    }

    @Test(expected = IllegalArgumentException.class)
    native public void persistNullObject() throws Exception;

    @Test
    native public void nativeGetPutAndToJSONString() throws Exception;

    @Test
    public void getPutAndToJSONString() throws Exception {

        MACRequests object = createMACRequests();
        object.put("parameter", "parameterValue");

        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_PARAMETER, object.get("parameter"));
        assertEquals(TestConstants.TEST_REQUEST, object.toJSONString());
    }

    @Test
    public void networkHttpPost() throws Exception {
        MACRequests object = createMACRequests();

        object.put("parameter", "parameterValue");
        object.setRequestType(MACRequests.HTTP_BIN);
        assertNotEquals(0, object.nPtr);

        HttpPost post = (HttpPost) object.getHttpPost();
        assertNotEquals(null, post);

        byte[] bytes = object.getBytes();
        assertNotEquals(null, bytes);
        String requestCertificate = new String(bytes);
        assertTrue(requestCertificate.contains("-----END CERTIFICATE-----"));


        String requestUrl = post.getURI().toString();
        assertEquals(TestConstants.TEST_URL, requestUrl);

        InputStream stream = post.getEntity().getContent();
        String content = new Scanner(stream).useDelimiter("\\A").next();
        assertNotEquals(null, content);
        JSONObject jsonObject = new JSONObject(content);

        String parameter = jsonObject.getString("parameter");
        assertEquals(TestConstants.TEST_PARAMETER, parameter);

        Header[] acceptHeaders = post.getHeaders("Accept");
        String acceptString = acceptHeaders[0].getValue();

        Header[] contentHeaders = post.getHeaders("Content-Type");
        String contentString = contentHeaders[0].getValue();

    }

    @Test
    public void networkRequest() throws Exception {
        MACRequests object = MACRequests.getInstance();
        assertNotEquals(0, object.nPtr);

        object.put("key", "1234");
        object.request(MACRequests.HTTP_BIN);

        assertNotNull(object.resultString);
        JSONObject jsonObject = new JSONObject(object.resultString);
        String requestUrl = jsonObject.getString("url");
        assertEquals(TestConstants.TEST_URL, requestUrl);

        JSONObject headers = jsonObject.getJSONObject("headers");

        String acceptString = headers.getString("Accept");
        assertEquals(TestConstants.TEST_HEADERS, acceptString);

        String contentString = headers.getString("Content-Type");
        assertEquals(TestConstants.TEST_HEADERS, contentString);
    }
    @Test
    native public void nativeMACRequestsRequest() throws Exception;

    @Test
    public void destroyMACRequests() throws Exception {
        MACRequests object = createMACRequests();
        assertNotEquals(0, object.nPtr);
        assertEquals(null, object.resultString);

        destroyMACRequests(object);

        org.junit.Assert.assertEquals(0, object.nPtr);
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
        MACRequests object = createMACRequests();
        assertNotEquals(0, object.nPtr);
        object.destroy();
    }
}
