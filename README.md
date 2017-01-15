# AndroidJniHelpers
Tools for writing secure Android/JNI code, based upon Spotify's [Jni Helpers Library](https://github.com/spotify/JniHelpers.git).

The Android Jni Helpers library can hide important details of your app from
basic apk decompilation, and harden your app beyond [basic code shrinking](https://developer.android.com/studio/build/shrink-code.html)
from [proguard obfuscation](https://www.guardsquare.com/en/proguard). Here are some of our current features:

**Java String Decoding**
```java

    DecodedString object = DecodedString.getInstance();
    object.encodedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    String decodedString = object.decode();
    assertEquals("To be or not to be, That is the question", decodedString);

```
**Native String Decoding**
```c++

    DecodedString *object = new DecodedString(env);
    object->encodedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    jstring decodedString = object->decode(env);
    const char * theString = env->GetStringUTFChars(env, string, 0);
    JUNIT_ASSERT_EQUALS_STRING("To be or not to be, That is the question", theString);

```
**The decode has a hidden implementation, and can pretty much be any algorithm you choose**

Planned Features:

**Native Secure Network Calls**
```java
var s = "JavaScript syntax highlighting";
alert(s);
```

**Easy Replication of Java APIs**
```java
var s = "JavaScript syntax highlighting";
alert(s);
```

**Easy Replication of Java APIs**
```java
var s = "JavaScript syntax highlighting";
alert(s);
```

##Setting up your project
* Open your command line
* cd into your C++/Android project (you can add C++ support through the content menu)
* Clone the repo into your project:
```bash
git clone https://github.com/the-mac/AndroidJniHelpers.git
```
* Go to File > New > Import Module
* Add AndroidJniHelpers to your project (you could name its module library)
* Rewrite the app/CMakeLists.txt similar to the following (you can find an example [here](AndroidJniHelpers/library/CMakeLists.txt)):
```cmake
cmake_minimum_required(VERSION 3.4.1)

set(library_JniHelpers_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../AndroiJniHrelpers/library/src/main/cpp)
set(library_JniHelpersTest_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../AndroiJniHrelpers/library/src/androidTest/cpp)

file(GLOB libJniHelpers_SOURCES ${library_JniHelpers_DIR}/*.cpp)
file(GLOB libJniHelpers_HEADERS ${library_JniHelpers_DIR}/*.h)

file(GLOB libJniHelpersTest_SOURCES ${library_JniHelpersTest_DIR}/*.cpp)
file(GLOB libJniHelpersTest_HEADERS ${library_JniHelpersTest_DIR}/*.h)

link_directories(${JNI_LIBRARIES})
add_library(JniHelpers STATIC ${libJniHelpers_SOURCES})
add_library(JniHelpersTest STATIC ${libJniHelpers_SOURCES} ${libJniHelpersTest_SOURCES})
include_directories(${library_JniHelpers_DIR} ${library_JniHelpersTest_DIR})


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
using the test-lib reference above (inside
androidTest/cpp folder). Also the
ANDROID_TESTING flag is passed in from the
build.gradle script, check out an example of
that [here](AndroidJniHelpers/library/build.gradle)**
