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

import java.io.InputStream;
import java.util.Scanner;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

public class NetworkTest {
    static {
        System.loadLibrary("test-helper-lib");
    }

    native public Network createNetwork();

    native public Network getPersistedInstance(Network object);

    native public void destroyNetwork(Network object);

    @Test
    public void persist() throws Exception {
        Network object = createNetwork();
        assertNotEquals(0, object.nPtr);
        // These properties should be set by the first native method in this case
        assertEquals(TestConstants.TEST_RESULT, object.resultString);

        // Now create a new empty object, but copy the nPtr field to it. Note that
        // the i field is *not* copied; that value is stored natively and should
        // be retrieved in the call to getPersistedInstance.
        Network emptyInstance = Network.testingDefault();
        emptyInstance.nPtr = object.nPtr;

        // The native test should be able to fetch the previous instance via nPtr,
        // and return to us the same instance data in a new object.
        Network result = getPersistedInstance(emptyInstance);
        assertEquals(object.nPtr, result.nPtr);
        assertEquals(object.resultString, result.resultString);

        // Always clean up after yourself, kids!
        destroyNetwork(object);
    }

    @Test(expected = IllegalArgumentException.class)
    native public void persistNullObject() throws Exception;

    @Test
    native public void nativeGetPutAndToJSONString() throws Exception;

    @Test
    public void getPutAndToJSONString() throws Exception {

        Network object = createNetwork();
        object.put("parameter", "parameterValue");

        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_PARAMETER, object.get("parameter"));
        assertEquals(TestConstants.TEST_REQUEST, object.toJSONString());
    }

    @Test
    public void networkHttpPost() throws Exception {
        Network object = Network.getInstance();

        object.put("parameter", "parameterValue");
        object.setRequestType(Network.HTTP_BIN);

        HttpPost post = (HttpPost) object.getHttpPost();
        assertNotEquals(0, object.nPtr);
        assertNotEquals(null, post);


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
//        Network object = createNetwork();
        Network object = Network.getInstance();
        assertNotEquals(0, object.nPtr);

        object.setReel("1234");
        object.request(Network.HTTP_BIN);

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
    native public void nativeNetworkRequest() throws Exception;

    @Test
    public void destroyNetwork() throws Exception {
        Network object = createNetwork();
        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_RESULT, object.resultString);

        destroyNetwork(object);

        assertEquals(0, object.nPtr);
        // Destroy should only alter the nPtr field, this should remain untouched
        assertEquals(TestConstants.TEST_RESULT, object.resultString);
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
        Network object = createNetwork();
        assertNotEquals(0, object.nPtr);
        object.destroy();
    }
}
