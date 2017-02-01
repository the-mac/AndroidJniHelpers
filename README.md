# AndroidJniHelpers
Tools for writing secure Android/JNI code, based upon Spotify's [Jni Helpers Library](https://github.com/spotify/JniHelpers.git).

The Android Jni Helpers library can hide important details of your app from basic apk decompilation. Here is a [demonstration](https://www.youtube.com/watch?v=TfLq9nsLWOc)  of how easy its done using [javadecompilers](http://www.javadecompilers.com/apk). To harden your app beyond [basic code shrinking](https://developer.android.com/studio/build/shrink-code.html)
from [proguard obfuscation](https://www.guardsquare.com/en/proguard), here are a few of our current features:

**Java String Decoding**
```java

    EncodedString object = EncodedString.getInstance();
    object.encodedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    String decodedString = object.decrypt(EncodedString.INLINE_STRINGS_ALGORITHM);
    assertEquals("To be or not to be, That is the question", decodedString);

```
**Native String Decoding**
```c++

    EncodedString *object = new EncodedString(env);
    object->encodedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    JavaString decryptedString(env, object->decryptNative(env, EncodedString::INLINE_STRINGS_ALGORITHM));
    JUNIT_ASSERT_EQUALS_STRING("To be or not to be, That is the question", decodedString.get());

```
**This allows you to encode your inline strings, and hide important details of your app. The decode has a hidden implementation, and can pretty much be any algorithm you choose**

**Java Secure Network Calls**
```java

    Network object = Network.getInstance();
    object.put("key", "1234");
    object.request(Network.HTTP_BIN);

    JSONObject jsonObject = new JSONObject(object.resultString);
    String requestUrl = jsonObject.getString("url");
    assertEquals("https://httpbin.org/post", requestUrl);


```
**Native Secure Network Calls**
```c++

    Network *network = new Network(env);
    jstring response = network->request(env, Network::HTTP_BIN);
    std::string resultString = JavaString(env, response).get();

    JSONObject jsonObject(env, resultString);
    JavaString requestUrl(env, jsonObject.getString(env, "url"));
    JUNIT_ASSERT_EQUALS_STRING("https://httpbin.org/post", requestUrl.get());

```

#Setting up your project
* Open your command line
* cd into your C++/Android project (you can add C++ support through the Android Studio context menu)
* Clone the repo into your project:
```bash
git clone https://github.com/the-mac/AndroidJniHelpers.git
```
* Go to File > New > Import Module
* Add AndroidJniHelpers to your project (you could name its module library)
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