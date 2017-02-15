package us.the.mac.android.jni.helpers;

import android.app.Application;
import android.util.Log;

/**
 * Created by christopher on 2/6/17.
 */
public class AndroidJniApp extends Application {

    private static AndroidJniApp Instance;
    public AndroidJniApp() { Instance = this; }
    public static AndroidJniApp Instance() { return Instance; }

    public native String decryptString(int resource);

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
}
