package us.the.mac.android.jni.helpers;

import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.preference.PreferenceManager;
import android.util.Log;

import com.spotify.jni.NativeObject;
import com.spotify.jni.annotations.UsedByNativeCode;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.conn.ssl.X509HostnameVerifier;
import org.apache.http.impl.client.DefaultHttpClient;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.concurrent.ExecutionException;

import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLException;
import javax.net.ssl.SSLSession;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.TrustManagerFactory;

public class Network extends NativeObject {

    private static final int BASE = 0;
    private static final int INCREMENT = 1;

    public static final int HTTP_BIN = BASE + INCREMENT;
    public static final int JSON_TEST = HTTP_BIN + INCREMENT;
    public static final int POSTER = JSON_TEST + INCREMENT;
    public static final int SERVICE = POSTER + INCREMENT;
    public static final int SAVE = SERVICE + INCREMENT;
    public static final int SETUP = SAVE + INCREMENT;

    private static final String A = "\\A";
    private static final String X_509 = "X.509";
    private static final String CA2 = "ca";
    private static final String TLS = "TLS";
    private static final String HTTPS = "https";

    private int retryCount = 3;
    String resultString;

    public static native Network getInstance();
    static Network testingDefault() { return new Network(); }
    private Network() {}

//
//    /**
//     * Called to save supplied value in shared preferences against given key.
//     * @param context Context of caller activity
//     * @param key Key of value to save against
//     * @param value Value to save
//     */
//    public void saveToPrefs(String key, String value) {
//        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(ReelApplication.Instance());
//        final SharedPreferences.Editor editor = prefs.edit();
//        editor.putString(key,value);
//        editor.commit();
//    }
//
//    /**
//     * Called to retrieve required value from shared preferences, identified by given key.
//     * Default value will be returned of no value found or error occurred.
//     * @param context Context of caller activity
//     * @param key Key to find value against
//     * @param defaultValue Value to return if no data found against given key
//     * @return Return the value found against given key, default if not found or any error occurs
//     */
//    public String getFromPrefs(String key, String defaultValue) {
//        SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(ReelApplication.Instance());
//        try {
//            return sharedPrefs.getString(key, defaultValue);
//        } catch (Exception e) {
//            e.printStackTrace();
//            return defaultValue;
//        }
//    }
//
//    /**
//     *
//     * @param context Context of caller activity
//     * @param key Key to delete from SharedPreferences
//     */
//    public void removeFromPrefs(String key) {
//        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(ReelApplication.Instance());
//        final SharedPreferences.Editor editor = prefs.edit();
//        editor.remove(key);
//        editor.commit();
//    }

    private static final String REEL = "reel";
    private static final String SERVICE_KEY = "service";
    private static final String API_KEY = "apiKey";
    private static final String REEL_ID = "reel_id";
    private static final String STATUS = "status";
    private static final String VERSION = "version";

//    public Network setKey(String key) {
//        put(API_KEY, key);
//        return this;
//    }
//    public Network setService(String service) {
//        put(SERVICE_KEY, service);
//        return this;
//    }
//    public Network setReel(String reel) {
//        put(REEL, reel);
//        return this;
//    }
//    public Network setReelId(String reel) {
//        put(REEL_ID, reel);
//        return this;
//    }
//    public Network setStatus(String status) {
//        put(STATUS, status);
//        return this;
//    }
//    public Network setVersion(String version) {
//        put(VERSION, version);
//        return this;
//    }

    @SuppressWarnings("UnusedDeclaration")
    @UsedByNativeCode
    public String getResultString() {
        return resultString;
    }

    @SuppressWarnings("UnusedDeclaration")
    @UsedByNativeCode
    public void setResultString(String e) {
        this.resultString = e;
    }

    @Override
    public native void destroy();

    public native byte[] getBytes();
    public native Object getHttpPost();

    public native void put(String key, String value);
    public native String get(String key);
    public native String toJSONString();
    public native void setRequestType(int requestType);

    public String request(int requestType) {

        setRequestType(requestType);

        String data = null;
        try { data = getResult(); }
        catch(Exception e) { new Exception(String.format("retryCount: %d, requestJsonObject: %s, data: %s", retryCount, toJSONString(), data), e).printStackTrace(); }

        if(data == null && retryCount-- > 0) { return request(requestType); }
        Log.e(getClass().getName(), String.format("retryCount: %d, requestJsonObject: %s,\n response length: %d, jsonObject: %s", retryCount, toJSONString(), data == null ? 0 : data.length(), data));

        return data;
    }
    public String getResult() throws InterruptedException, ExecutionException {
        return new AsyncTask<Void,Void,String>() {
            @Override protected String doInBackground(Void... params) {
                try {
                    HttpResponse response = getSSLClient().execute((HttpPost)getHttpPost());
                    InputStream in = response.getEntity().getContent();
                    return new java.util.Scanner(in).useDelimiter(A).next();
                }
                catch (ClientProtocolException e) { e.printStackTrace(); }
                catch (IOException e) { e.printStackTrace(); }
                return null;
            }

            @Override protected void onPostExecute(String s) {
                super.onPostExecute(s);
                setResultString(s);
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR).get();
    }

    private DefaultHttpClient getSSLClient() {
        DefaultHttpClient client = new DefaultHttpClient();
        try {

            CertificateFactory cf = CertificateFactory.getInstance(X_509);
            InputStream caInput = new ByteArrayInputStream(getBytes());
            Certificate ca = null;
            try { ca = cf.generateCertificate(caInput); }
            catch (Exception e) { e.printStackTrace(); }

            // Create a KeyStore containing our trusted CAs
            String keyStoreType = KeyStore.getDefaultType();
            KeyStore keyStore = KeyStore.getInstance(keyStoreType);
            keyStore.load(null, null);
            keyStore.setCertificateEntry(CA2, ca);

            // Create a TrustManager that trusts the CAs in our KeyStore
            String tmfAlgorithm = TrustManagerFactory.getDefaultAlgorithm();
            TrustManagerFactory tmf = TrustManagerFactory.getInstance(tmfAlgorithm);
            tmf.init(keyStore);

            SSLContext ctx = SSLContext.getInstance(TLS);
            ctx.init(null, tmf.getTrustManagers(), null);

            SSLSocketFactory ssf = new SSLSocketFactory(ctx);
            ssf.setHostnameVerifier(new X509HostnameVerifier() {
                @Override public boolean verify(String hostname, SSLSession session) { return false; }
                @Override public void verify(String host, SSLSocket ssl) throws IOException { }
                @Override public void verify(String host, X509Certificate cert) throws SSLException { }
                @Override public void verify(String host, String[] cns, String[] subjectAlts) throws SSLException { }
            });

            ClientConnectionManager ccm = client.getConnectionManager();
            SchemeRegistry sr = ccm.getSchemeRegistry();
            sr.register(new Scheme(HTTPS, ssf, 443));

            return new DefaultHttpClient(ccm, client.getParams());
        } catch (Exception ex) {
            return null;
        }
    }

    static class SSLSocketFactory extends org.apache.http.conn.ssl.SSLSocketFactory {
        SSLContext sslContext = SSLContext.getInstance(TLS);

        public SSLSocketFactory(KeyStore truststore) throws NoSuchAlgorithmException, KeyManagementException, KeyStoreException, UnrecoverableKeyException {
            super(truststore);
        }

        public SSLSocketFactory(SSLContext context) throws KeyManagementException, NoSuchAlgorithmException, KeyStoreException, UnrecoverableKeyException {
            super(null);
            sslContext = context;
        }

        @Override
        public Socket createSocket(Socket socket, String host, int port, boolean autoClose) throws IOException, UnknownHostException {
            return sslContext.getSocketFactory().createSocket(socket, host, port, autoClose);
        }

        @Override
        public Socket createSocket() throws IOException {
            return sslContext.getSocketFactory().createSocket();
        }
    }

}
