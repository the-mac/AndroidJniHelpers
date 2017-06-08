package us.the.mac.android.jni.helpers;

import android.app.ActivityManager;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.List;

/**
 * Created by christopher on 2/6/17.
 */
public class AndroidJniApp extends Application {

    public static Intent mTestingBundle;
    private static AndroidJniApp Instance;

    public AndroidJniApp() { Instance = this; }
    public static AndroidJniApp Instance() { return Instance; }

    public static String getProcessName(Context context) {
        final File cmdline = new File("/proc/" + android.os.Process.myPid() + "/cmdline");
        try (BufferedReader reader = new BufferedReader(new FileReader(cmdline))) {
            return reader.readLine();
        } catch (IOException e) { return null; }
    }
    public static boolean isServiceTest() {
        try {
            String processName = getProcessName(Instance());
            boolean isServiceTest = processName.contains("services.test");

            Log.d(AndroidJniApp.class.getName(), "Called isServiceTest = "+isServiceTest);
            return isServiceTest;
        }
        catch (final Exception e) {
            Log.d(AndroidJniApp.class.getName(), "Called isServiceTest defaulting isServiceTest = "+false);
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

}
