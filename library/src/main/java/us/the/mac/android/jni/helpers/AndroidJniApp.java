package us.the.mac.android.jni.helpers;

import android.app.Application;
import android.content.Intent;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.InputStream;

/**
 * Created by christopher on 2/6/17.
 */
public class AndroidJniApp extends Application {

    public static Intent mTestingBundle;
    private static AndroidJniApp Instance;

    public AndroidJniApp() { Instance = this; }
    public static AndroidJniApp Instance() { return Instance; }
    public static boolean isJUnitTest() {
        try {
            boolean isJUnitTest = mTestingBundle.hasExtra("isJUnitTest");
            Log.d(AndroidJniApp.class.getName(), "isJUnitTest = "+isJUnitTest);
            return isJUnitTest;
        }
        catch (final Exception e) {
            Log.d(AndroidJniApp.class.getName(), "Defaulting isJUnitTest = "+false);
            return false;
        }
    }

//    public native String decryptString(int resource);
    public static native String getEncrypted(int position);
    public static native String getS(int position);

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(getClass().getName(), "Called AndroidJniApp.onCreate");
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
        Log.d(getClass().getName(), "Called AndroidJniApp.onLowMemory");
    }

    @Override
    public void onTerminate() {
        super.onTerminate();
        Log.d(getClass().getName(), "Called AndroidJniApp.onTerminate");
    }
    public String getFilesDirectory() { return Instance().getFilesDir().toString(); }

    public InputStream openAssetFileInput(String file) {
        try {
            AssetManager assetManager = AndroidJniApp.Instance().getAssets();
            return assetManager.open(file);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

//    static {
//        System.loadLibrary(isJUnitTest() ? "test-helper-lib" : "jni-helper-lib");
//    }
}
