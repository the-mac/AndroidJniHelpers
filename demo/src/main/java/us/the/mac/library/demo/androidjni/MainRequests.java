package us.the.mac.library.demo.androidjni;

import us.the.mac.android.jni.helpers.Network;

/**
 * Created by donnell on 1/20/17.
 */

public class MainRequests extends Network {

    private static final int BASE = 0;
    private static final int INCREMENT = 1;

    public static final int HTTP_BIN = BASE + INCREMENT;
    public static final int JSON_TEST = HTTP_BIN + INCREMENT;

    private static final String API_KEY = "apiKey";

    public static native MainRequests getInstance();

    public MainRequests setKey(String key) {
        put(API_KEY, key);
        return this;
    }
}