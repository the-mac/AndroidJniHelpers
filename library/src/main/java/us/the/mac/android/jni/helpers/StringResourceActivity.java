/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package us.the.mac.android.jni.helpers;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

/**
 * Displays an Android textView widget backed by data in an array. The
 * array is loaded from the strings.xml resources file.
 */
public class StringResourceActivity extends Activity {
    /**
     * Initializes the application and the activity.
     * 1) Sets the view
     * 2) Reads the textView's backing data from the string resources file
     * 3) Instantiates a callback listener for handling selection from the
     *    textView
     * Notice that this method includes code that can be uncommented to force
     * tests to fail.
     *
     * This method overrides the default onCreate() method for an Activity.
     *
     * @see Activity#onCreate(Bundle)
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {

        /**
         * derived classes that use onCreate() overrides must always call the super constructor
         */
        super.onCreate(savedInstanceState);

        setContentView(R.layout.string_resource_main);

        TextView textView = (TextView) findViewById(R.id.textView);
        textView.setText(decryptString(R.string.encryptedString));

    }

    public native String decryptString(int encryptedString);
}
