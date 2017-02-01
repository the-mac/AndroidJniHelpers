package us.the.mac.android.jni.helpers;

/**
 * Created by donnell on 1/20/17.
 */

public class MACRequests extends Network {

    private static final int BASE = 0;
    private static final int INCREMENT = 1;

    public static final int HTTP_BIN = BASE + INCREMENT;
    public static final int JSON_TEST = HTTP_BIN + INCREMENT;

    private static final String API_KEY = "apiKey";

    public static native MACRequests getInstance();

    public static MACRequests testingDefault() { return new MACRequests(); }

    public MACRequests setKey(String key) {
        put(API_KEY, key);
        return this;
    }
}