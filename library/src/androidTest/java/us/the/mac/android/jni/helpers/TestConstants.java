package us.the.mac.android.jni.helpers;

/**
 * Common values for tests which pass objects through JNI.
 * NB: This file is GENERATED. DO NOT EDIT.
 */
public class TestConstants {
    public static final String TEST_URL = "https://httpbin.org/post";
    public static final String TEST_REQUEST = "{ \"parameter\": \"parameterValue\" }";
    public static final String TEST_PARAMETER = "parameterValue";
    public static final String TEST_RESULT = "{ \"args\": {}, \"data\": \"\", \"files\": {}, \"form\": {}, \"headers\": { \"Accept\": \": application/json\", \"Content-Length\": \"0\", \"Content-Type\": \"application/json\", \"Host\": \"httpbin.org\", \"User-Agent\": \"Apache-HttpClient/UNAVAILABLE (java 1.4)\" }, \"json\": null, \"origin\": \"172.58.153.6\", \"url\": \"https://httpbin.org/post\" } ";
    public static final String TEST_HEADERS = "application/json";
    public static final String TEST_ENCRYPTED = "Up cf ps opu up cf, Uibu jt uif rvftujpo";
    public static final String TEST_ENCRYPTION_KEY = "XYP4bRwEWhNgaX8DSYCAAg==\n";
    public static final String TEST_ENCRYPTED_RESOURCE = "OUI3ZGR6N3RtOHV6V2RwcENpQ1ZGZHZvc3grV2NNOFN4eEFJdDJPR1YwUVJpYStUVDR5R1RQdE8wVHN5dVVSTw==";
    public static final String TEST_DECRYPT = "To be or not to be, That is the question";
    public static final String TEST_BASE64_DECRYPT = "tO BE OR NOT TO BE, tHAT IS THE QUESTION";
    public static final String TEST_STRING = "hello, is it me you're looking for?";
    public static final String TEST_NULL_CHAR = "\0";
    public static final String TEST_STRING_WITH_NULL_CHAR = "hello \0 world";
    public static final String TEST_UTF16_STRING = "UTF16 hello: \u2018.";
    public static final String TEST_UTF8_STRING = "a \u0915\u093E\u091A\u0902 \u00F6 1 2 3";
    public static final int TEST_INTEGER = 42;
    public static final short TEST_SHORT = 777;
    public static final boolean TEST_BOOLEAN = true;
    public static final float TEST_FLOAT = 3.14159f;
    public static final double TEST_FLOAT_TOLERANCE = 0.01;
    public static final double TEST_DOUBLE = 2.71828;
    public static final byte TEST_BYTE = 72;
    public static final char TEST_CHAR = 'ö';
}
