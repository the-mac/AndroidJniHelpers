package us.the.mac.library.demo.androidjni;

import android.app.IntentService;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.text.format.DateFormat;
import android.util.Log;
import android.widget.Toast;

import us.the.mac.android.jni.helpers.AndroidJniApp;

public class MainIntentService extends IntentService {
    public static final String PARAM_IN_MSG = "imsg";
    public static final String WELCOME_MESSAGE = AndroidJniApp.getS(0);
 
    public MainIntentService() {
        super("MainIntentService");
        Log.d(getClass().getName(), "Called constructor successfully");
    }
    @Override public void onCreate() {
        super.onCreate();
        Log.d(getClass().getName(), "Called onCreate successfully");
    }
    @Override protected void onHandleIntent(Intent intent) {
        String input = intent.getStringExtra(PARAM_IN_MSG);
        Log.d(getClass().getName(), "Called onHandleIntent successfully with: "+ input);

        SystemClock.sleep(3000); // 3 seconds
        showToast(input + " " + DateFormat.format("MM/dd/yy h:mmaa", System.currentTimeMillis()));
    }
    public void showToast(final String message) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override public void run() { Toast.makeText(getApplicationContext(), message, Toast.LENGTH_LONG).show(); }
        });
    }

    static {
        Log.d(MainIntentService.class.getName(), "Called static {...} successfully");
        // SET AUTHORITY: us.the.mac.demo.library.androidjni.provider
    }
}