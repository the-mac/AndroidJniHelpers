# Android JNI Helpers #

You may already know, but in Android development we need to hide important details of our apps from basic apk decompilation. Here is a [demonstration](https://www.youtube.com/watch?v=TfLq9nsLWOc)  of how easy apk decompilation truly is, using the online decompiler [javadecompilers](http://www.javadecompilers.com/apk).


[![Decompile APK in Single Click](https://img.youtube.com/vi/TfLq9nsLWOc/0.jpg)](https://www.youtube.com/watch?v=TfLq9nsLWOc)


The Android JNI Helpers library allows you to secure your apps with less effort and is fully customizable. With this version of the JNI Helpers library you can prepare your strings before runtime and decrypt them as you need them, and connect to any secure server with its designated ssl certificate (in C++). It protects your app's resources like inline (Java file) strings, resource (res) strings and network requests beyond
[basic code shrinking](https://developer.android.com/studio/build/shrink-code.html) from [proguard obfuscation](https://www.guardsquare.com/en/proguard).

## Using String Decryption ##

![Android JNI Encryption Preview](screenshots/android-jni-encryption-preview.png)

To get started, you would add string resource encryption (to your strings.xml)
```xml
<resources>
    <string name="app_name">Android Jni Helpers</string>
    <string name="action_settings">Settings</string>
    <string name="encryptedString">HiYJ+OroGm0hwPv5hz9V0g==</string>
    ...
</resources>
```

Next, you must declare the JNI method (in Java) then implement it (in C/C++)
```Java
package us.the.mac.example.app;
...
public class ExampleActivity extends Activity {
    ...
    public native String decryptString(int resource);
}
```

Next, to allow the decryptString Java method to work the C++ implmentation should look like the following:
```C++
extern "C"
jstring
Java_us_the_mac_example_app_ExampleActivity_decryptString(JNIEnv* env, jobject java_this, jint resourceId) {
    // REQUEST STRING RESOURCE FROM strings.xml
    jstring stringResource = getStringNative(env, java_this, resourceId);

    // THIS IS YOUR CUSTOM ENCRYPTED STRING CLASS
    YourEncryptedString es = YourEncryptedString(env);
    es.encryptedString = env->GetStringUTFChars(stringResource, JNI_FALSE);

    // USE AN ALGORITHM OF YOUR CHOICE TO DECRYPT HERE
    return es.decryptNative(env, YourEncryptedString::RESOURCE_STRINGS_ALGORITHM);
}
```

Finally, request the decrypted string in the example Activity:
```Java
    @Override
    public void onCreate(Bundle bundle) {

        super.onCreate(bundle);
        setContentView(R.layout.string_resource_main);

        TextView textView = (TextView) findViewById(R.id.textView);
        textView.setText(decryptString(R.string.encryptedString));

    }
```

Similar files to the examples above can be found in the [demo](demo) project's [Activity](demo/src/main/java/us/the/mac/library/demo/androidjni/MainActivity.java) and [C++ code](demo/src/main/cpp/native-lib.cpp)

## Using Secure Network Calls ##

![Android JNI Network Preview](screenshots/android-jni-network-preview.png)

Create a custom Network Java subclass
```Java
public class MainRequests extends Network {

    private static final int BASE = 0;
    private static final int INCREMENT = 1;

    public static final int HTTP_BIN = BASE + INCREMENT;
    public static final int JSON_TEST = HTTP_BIN + INCREMENT;

    private static final String API_KEY = "apiKey";
    public static native MainRequests getInstance();

    public MainRequests setKey(String key) {
        put(API_KEY, key);
        return this;
    }
}
```

Create a custom Network C++ subclass
```C++

class MainRequests : public Network {

protected:
    std::string requestSession;

public:

    MainRequests(JNIEnv *env);
    static jobject getInstance(JNIEnv *env, jobject java_this);

    static const int BASE = 0;
    static const int INCREMENT = 1;
    static const int HTTP_BIN = BASE + INCREMENT;
    static const int JSON_TEST = HTTP_BIN + INCREMENT;

    static const char * const CANONICAL_CLASS_NAME;
};
```

Finally, using a MainRequests Java instance could look as follows:
```Java
    MainRequests object = MainRequests.getInstance().setKey("1234");
    object.request(MainRequests.HTTP_BIN);

    JSONObject jsonObject = new JSONObject(object.resultString);
```
Similar files to the examples above can be found in the [demo](demo) project's MainRequests [java file](demo/src/main/java/us/the/mac/library/demo/androidjni/MainRequests.java) and [header file](demo/src/main/cpp/MainRequests.h).

## Setting Up Your Project ##
### Add C++ Support, but if your project already has it skip this section ###
- If you are just creating your project, you can simply check the "Add C++ Support" box upon project creation
- If your project already exists, you can right click your app module and select "Link C++ project with gradle"
- Finally, see our [FAQ](FAQ.md) for how to [set up your C++ project and view example CMakeLists.txt file](FAQ.md#setting-up-your-android-studio-c-project)

## ProGuard
Nothing to include

## Contributing
Want to contribute? Great! First, read the [CONTRIBUTING](CONTRIBUTING.md) page (including the small print at the end).
