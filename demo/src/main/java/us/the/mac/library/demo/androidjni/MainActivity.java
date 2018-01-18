package us.the.mac.library.demo.androidjni;

import android.os.SystemClock;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.format.DateFormat;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import us.the.mac.android.jni.helpers.AndroidJniApp;

public class MainActivity extends AppCompatActivity {
    static {
        Log.d(MainActivity.class.getName(), "Called static {...} successfully");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI()+"\n\n"+ AndroidJniApp.getS(0));

        Log.d(getClass().getName(), "Called onCreate successfully");
    }

    @Override
    protected void onPause() {
        super.onPause();
        PreferenceManager.getDefaultSharedPreferences(this).edit().clear().commit();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public void sendRequest(View view) {
        Log.d(getClass().getName(), "Called sendRequest with view: "+ view);

        String result = MainRequests.getInstance().setKey("testKeyData").request(MainRequests.HTTP_BIN);

        Log.d(getClass().getName(), "Called sendRequest successfully with result: "+ result);

        SystemClock.sleep(3000); // 3 seconds
        Toast.makeText(this, result + " " + DateFormat.format("MM/dd/yy h:mmaa", System.currentTimeMillis()), Toast.LENGTH_LONG).show();
    }
}
