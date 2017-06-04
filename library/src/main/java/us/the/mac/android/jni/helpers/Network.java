package us.the.mac.android.jni.helpers;

import android.os.AsyncTask;
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

import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLException;
import javax.net.ssl.SSLHandshakeException;
import javax.net.ssl.SSLSession;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.TrustManagerFactory;

public class Network extends NativeObject {

    private static final int BASE = 0;
    private static final int INCREMENT = 1;

    private static final String A = "\\A";
    private static final String X_509 = "X.509";
    private static final String CA2 = "ca";
    private static final String TLS = "TLS";
    private static final String HTTPS = "https";

    private int retryCountRemaining = 3;
    public String resultString;

    protected Network() {}

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
    public native HttpPost getHttpPost();

    public native void put(String key, String value);
    public native String get(String key);
    public native String toJSONString();
    public native void setRequestType(int requestType);

    public String request(int requestType) {

        setRequestType(requestType);

        String data = null;
        try { data = getResult(); }//new JSONObject(getResult()).toString(); }
        catch(SSLHandshakeException e) { new Exception(String.format("Please check for your certificate in getBytes"), e).printStackTrace(); }
        catch(Exception e) { new Exception(String.format("retryCountRemaining: %d, requestJsonObject: %s, data: %s", retryCountRemaining, toJSONString(), data), e).printStackTrace(); }

        if(data == null && retryCountRemaining-- > 0) { return request(requestType); }
        Log.e(getClass().getName(), String.format("Called request retryCountRemaining: %d, requestJsonObject: %s,\n response length: %d, jsonObject: %s\n\n", retryCountRemaining, toJSONString(), data == null ? 0 : data.length(), data));

        return data;
    }
    public String getResult() throws Exception {
        return new AsyncTask<Void,Void,String>() {
            @Override protected String doInBackground(Void... params) {
                try {
                    HttpResponse response = getSSLClient().execute(getHttpPost());
                    InputStream in = response.getEntity().getContent();
                    return new java.util.Scanner(in).useDelimiter(A).next();
                }
                catch (ClientProtocolException e) { e.printStackTrace(); return e.getMessage(); }
                catch (Exception e) { e.printStackTrace(); return e.getMessage(); }
            }

            @Override protected void onPostExecute(String s) {
                super.onPostExecute(s);
                setResultString(s);
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR).get();
    }

    private DefaultHttpClient getSSLClient() {
        DefaultHttpClient client = new DefaultHttpClient();
        CertificateFactory cf = null;
        InputStream caInput = null;
        Certificate ca = null;
        try {

            cf = CertificateFactory.getInstance(X_509);
            caInput = new ByteArrayInputStream(getBytes());

            try { ca = cf.generateCertificate(caInput); }
            catch (Exception e) {
                e.printStackTrace();
                return client;
            }

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
        }
        catch(Exception e) {
            new Exception(String.format("cf: %s, caInput: %s, ca: %s", cf, caInput, ca), e).printStackTrace();
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
