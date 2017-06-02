package us.the.mac.android.jni.helpers;

import android.app.Application;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.InputStream;

/**
 * Created by christopher on 2/6/17.
 */
public class AndroidJniApp extends Application {

    private static AndroidJniApp Instance;
    public AndroidJniApp() { Instance = this; }
    public static AndroidJniApp Instance() { return Instance; }

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
}
