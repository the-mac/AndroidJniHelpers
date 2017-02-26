# AndroidJniHelpers
Tools for writing secure Android/JNI code, based upon Spotify's [Jni Helpers Library](https://github.com/spotify/JniHelpers.git).

The Android Jni Helpers library can hide important details of your app from basic apk decompilation. Here is
a [demonstration](https://www.youtube.com/watch?v=TfLq9nsLWOc)  of how easy apk decompilation truly is, using
the online decompiler [javadecompilers](http://www.javadecompilers.com/apk).

To harden your app's resources like inline (Java file) strings, resource (res) strings and network requests beyond
[basic code shrinking](https://developer.android.com/studio/build/shrink-code.html)
from [proguard obfuscation](https://www.guardsquare.com/en/proguard), here are a few of our solutions:

## String Decryption

**Incorporate String Resource Decrypting**
```java

    String decryptedString = decryptString(R.string.encryptedString);
    assertEquals("To be or not to be, That is the question", decryptedString);

```
**Above is an example of decrypting a generated resource string. If you would like to implement this, you can create a decryptString implementation for your Activity class**

**First, declare your native decryptString method in your Activity class**
```java

public class ExampleActivity extends Activity {
    ...
    public native String decryptString(int resource);
}

```

**Next, you will need to register the EncryptedString Java class with the Native Class Registry instance.**
```c++

ClassRegistry gClasses;
    ...
    gClasses.add(env, new EncryptedString(env));

```
**Lastly, you would implement the native decryptString method similar to the following:**
```c++

extern "C"
jstring
Java_us_the_mac_example_app_ExampleActivity_decryptString(JNIEnv* env, jobject java_this, jint resourceId) {
    jstring stringResource = getStringNative(env, java_this, resourceId);

    EncryptedString es = EncryptedString(env);
    es.encryptedString = env->GetStringUTFChars(stringResource, JNI_FALSE);

    return es.decryptNative(env, EncryptedString::RESOURCE_STRINGS_ALGORITHM);
}

```
**Coming Soon: AES Encrypted resource strings can be generated using the bin/encrypt.bash script.**

**Java Inline String Decrypting**
```java

    EncryptedString object = EncryptedString.getInstance();
    object.encryptedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    String decryptedString = object.decrypt(EncryptedString.INLINE_STRINGS_ALGORITHM);
    assertEquals("To be or not to be, That is the question", decryptedString);

```
**Native Inline String Decrypting**
```c++

    EncryptedString *object = new EncryptedString(env);
    object->encryptedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    JavaString decryptedString(env, object->decryptNative(env, EncryptedString::INLINE_STRINGS_ALGORITHM));
    JUNIT_ASSERT_EQUALS_STRING("To be or not to be, That is the question", decryptedString.get());

```
**This API allows you to encrypt your inline strings hiding important details of your app. The decrypt function is a hidden (Native C++) implementation, and can pretty much be any (Java, C, C++) decryption algorithm you choose**

## Secure Network Calls
**Java Secure Network Calls**
```java

public class MACRequests extends Network {

    private static final int BASE = 0;
    private static final int INCREMENT = 1;
    
    public static final int HTTP_BIN = BASE + INCREMENT;
    public static final int JSON_TEST = HTTP_BIN + INCREMENT;

    private static final String API_KEY = "apiKey";

    public static native MACRequests getInstance();

    public MACRequests setKey(String key) {
        put(API_KEY, key);
        return this;
    }
}

```
Using a MACRequests Java instance could look as follows:
```java

    MACRequests object = MACRequests.getInstance().setKey("1234");
    object.request(MACRequests.HTTP_BIN);

    JSONObject jsonObject = new JSONObject(object.resultString);
    String requestUrl = jsonObject.getString("url");
    assertEquals("https://httpbin.org/post", requestUrl);

```
**Native Secure Network Calls**
```c++

class MACRequests : public Network {

protected:
    std::string requestSession;

public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the MACRequests class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", MACRequests);
    }

    MACRequests();

    MACRequests(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

    /**
    * The getBytes method is used to get the Server's Certificate
    * @return SSL (Secure Sockets Layer) certificate bytes for a secure network connection.
    */
    static jbyteArray getBytes(JNIEnv *env, jobject java_this);

    /**
    * The setRequestType method is used to set the request data
    * In here you can set any variables needed for a specific session (url, session key, etc.)
    */
    static void setRequestType(JNIEnv *env, jobject java_this, jint jintValue1);

    /**
    * The getHttpPost method is used to return the request's post data (JSON by default)
    */
    static jobject getHttpPost(JNIEnv *env, jobject java_this);


    static jobject getInstance(JNIEnv *env, jobject java_this);


    static const int BASE = 0;
    static const int INCREMENT = 1;
    static const int HTTP_BIN = BASE + INCREMENT;
    static const int JSON_TEST = HTTP_BIN + INCREMENT;
};
```

For the MACRequests cpp file, see [AndroidJniHelper androidTest folder](library/src/androidTest/cpp/MACRequests.cpp)

Using a MACRequests native instance looks as follows:
```c++

    MACRequests *network = new MACRequests(env);
    network->putNative(env, env->NewStringUTF("apiKey"), env->NewStringUTF("d6f4"));
    jstring response = network->request(env, MACRequests::HTTP_BIN);
    std::string resultString = JavaString(env, response).get();

    JSONObject jsonObject(env, resultString);
    JavaString requestUrl(env, jsonObject.getString(env, "url"));
    JUNIT_ASSERT_EQUALS_STRING("https://httpbin.org/post", requestUrl.get());

```

#Setting up your Android Studio C++ Project

* Change directories into your C++/Android project:
```bash
cd ../path/to/your/project
```


**Note: If you don't have a C++ project, make sure to [add C++ support](https://developer.android.com/studio/projects/add-native-code.html#existing-project) to your project**
* To add C++ support to your project:
```bash
* Create new native source files (in cpp folder, beside java folder)

* Create a CMakeLists.txt build script (See add C++ support link above)

* Link Gradle to your native library:
android {
  ...
  defaultConfig {...}
  buildTypes {...}

  // Encapsulates your external native build configurations.
  externalNativeBuild {

    // Encapsulates your CMake build configurations.
    cmake {

      // Provides a relative path to your CMake build script.
      path "CMakeLists.txt"
    }
  }
}
```
* Clone the repo into your project:
```bash
git clone https://github.com/the-mac/AndroidJniHelpers.git
```
* Edit your project structure:
```bash
Go to File > Project Structure
```
* Add module into your project:
```bash
Click on "+" sign to add new module
```
* Import Gradle project:
```bash
Click "Import Gradle Project" option
```
* Add AndroidJniHelpers to your project (you could name its module library)
```bash
Paste "path/to/your/project/AndroidJniHelpers" folder into Source directory
```
* Name New Module:
```bash
By default the existing name should be library ":library", but can be updated
```

* Rewrite the app/CMakeLists.txt similar to the following (you can find an example [here](library/CMakeLists.txt)):
```cmake
cmake_minimum_required(VERSION 3.4.1)

set(local_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp)
set(library_JniHelpers_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../AndroidJniHelpers/library/src/main/cpp)
set(library_JniHelpersTest_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../AndroidJniHelpers/library/src/androidTest/cpp)

file(GLOB libLocal_SOURCES ${local_DIR}/*.cpp)
file(GLOB libLocal_HEADERS ${local_DIR}/*.h)

file(GLOB libJniHelpers_SOURCES ${library_JniHelpers_DIR}/*.cpp)
file(GLOB libJniHelpers_HEADERS ${library_JniHelpers_DIR}/*.h)

file(GLOB libJniHelpersTest_SOURCES ${library_JniHelpersTest_DIR}/*.cpp)
file(GLOB libJniHelpersTest_HEADERS ${library_JniHelpersTest_DIR}/*.h)

link_directories(${JNI_LIBRARIES})
add_library(JniHelpers STATIC ${libLocal_SOURCES} ${libJniHelpers_SOURCES})
add_library(JniHelpersTest STATIC ${libLocal_SOURCES} ${libJniHelpers_SOURCES} ${libJniHelpersTest_SOURCES})
include_directories(${local_DIR} ${library_JniHelpers_DIR} ${library_JniHelpersTest_DIR})


if(${ANDROID_TESTING})
    set(library_NAME "test-lib")
    set(library_Entry_Point "src/androidTest/cpp")
else()
    set(library_NAME "native-lib")
    set(library_Entry_Point "src/main/cpp")
endif()

add_library( # Sets the name of the library.
             ${library_NAME}

             # Sets the library as a shared library.
             SHARED

             # The following toggles between the native-lib and test-libs
             # files, based upon whehther a test is currently running.
             ${library_Entry_Point}/${library_NAME}.cpp )


find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


# Only adding JniHelpersTest library if testing
if(${ANDROID_TESTING})
    target_link_libraries(${library_NAME} JniHelpers JniHelpersTest ${log-lib} )
else()
    target_link_libraries(${library_NAME} JniHelpers ${log-lib} )
endif()
```
**Note: If you're creating JNI helper tests,
you should run them on an Android device and
using the test-lib reference above (inside your
androidTest/cpp folder). Also the
ANDROID_TESTING flag is passed in from the
build.gradle script, check out an example of
that [here](library/build.gradle)**

# Contributing

## Installing locally

You can download AndroidJniHelpers and install it locally by cloning this
repository and running:

    ./gradlew :library:prepareArtifacts

<!-- ###  Deployment -->

<!-- To deploy AndroidJniHelpers to Bintray -->

<!-- 1. Set `BINTRAY_USER` and `BINTRAY_KEY` in your environment. You must -->
<!-- be a member of the firebaseui Bintray organization. -->
<!-- 1. Run `./gradlew clean :library:prepareArtifacts :library:bintrayUploadAll` -->
<!-- 1. Go to the Bintray dashboard and click 'Publish'. -->
<!-- 1. In Bintray click the 'Maven Central' tab and publish the release. -->

## Tag a release on GitHub

* Ensure that all your changes are on master and that your local build is on master
<!-- * Ensure that the correct version number is in `common/constants.gradle` -->

## Contributor License Agreements

We'd love to accept your sample apps and patches! Before we can take them, we
have to jump a couple of legal hurdles.

Please fill out either the individual or corporate Contributor License Agreement
(CLA).

  * If you are an individual writing original source code and you're sure you
    own the intellectual property, then you'll need to sign an
    [individual CLA](https://developers.google.com/open-source/cla/individual).
  * If you work for a company that wants to allow you to contribute your work,
    then you'll need to sign a
    [corporate CLA](https://developers.google.com/open-source/cla/corporate).

Follow either of the two links above to access the appropriate CLA and
instructions for how to sign and return it. Once we receive it, we'll be able to
accept your pull requests.

## Contribution Process

1. Submit an issue describing your proposed change to the repo in question.
1. The repo owner will respond to your issue promptly.
1. If your proposed change is accepted, and you haven't already done so, sign a
   Contributor License Agreement (see details above).
1. Fork the desired repo, develop and test your code changes.
1. Ensure that your code adheres to the existing style of the library to which
   you are contributing.
1. Ensure that your code has an appropriate set of unit tests which all pass.
1. Submit a pull request and cc @cdm2012
