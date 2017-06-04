package us.the.mac.library.demo.androidjni;

import android.content.Intent;
import android.preference.PreferenceManager;
import android.util.Log;

import us.the.mac.android.jni.helpers.AndroidJniApp;

public class MainApplication extends AndroidJniApp {
    // Used to load the 'native-lib' library on application startup.
    static {
        Log.d(MainApplication.class.getName(), "Called static {...} successfully");
        System.loadLibrary("native-lib");
    }

    public MainApplication() {
        Log.d(getClass().getName(), "Called constructor successfully");
    }
    @Override public void onCreate() {
        super.onCreate();
        Log.d(getClass().getName(), "Called onCreate successfully");

        String strInputMsg = "Passing parameter to MainIntentService";
        Intent msgIntent = new Intent(this, MainIntentService.class);
        msgIntent.putExtra(MainIntentService.PARAM_IN_MSG, strInputMsg);
        startService(msgIntent);
    }
}