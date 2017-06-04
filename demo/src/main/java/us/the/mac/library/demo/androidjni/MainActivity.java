package us.the.mac.library.demo.androidjni;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import us.the.mac.android.jni.helpers.AndroidJniApp;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI()+"\n\n"+ AndroidJniApp.getS(0));

        Log.d(getClass().getName(), "Called onCreate successfully");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    static {
        Log.d(MainActivity.class.getName(), "Called static {...} successfully");
    }
}
