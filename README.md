# AndroidJniHelpers
Tools for writing secure Android/JNI code, based upon Spotify's [Jni Helpers Library](https://github.com/spotify/JniHelpers.git).

The Android Jni Helpers library can hide important details of your app from [basic apk decompilation](http://www.javadecompilers.com/apk). Here is a [demonstration of how easy its done using javadecompilers](https://www.youtube.com/watch?v=TfLq9nsLWOc). To harden your app beyond [basic code shrinking](https://developer.android.com/studio/build/shrink-code.html)
from [proguard obfuscation](https://www.guardsquare.com/en/proguard), here are a few of our current features:

**Java String Decoding**
```java

    EncodedString object = EncodedString.getInstance();
    object.encodedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    String decodedString = object.decode();
    assertEquals("To be or not to be, That is the question", decodedString);

```
**Native String Decoding**
```c++

    EncodedString *object = new EncodedString(env);
    object->encodedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    jstring decodedString = object->nativeDecode(env);
    const char * theString = env->GetStringUTFChars(env, string, 0);
    JUNIT_ASSERT_EQUALS_STRING("To be or not to be, That is the question", theString);

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

#Planned Features:

##Header Obfuscation (Inserted into main c/cpp file)

Before Obfuscation
```c++

//#include "obfuscate.h"
...

    Network *network = new Network(env);
    jstring response = network->request(env, Network::HTTP_BIN);
    std::string resultString = JavaString(env, response).get();

```

Generation of Obfuscation Header (obfuscate.h)
```c++

...
#define Network ghjRkl
#define network xrdtcf
#define env afh
#define response sdfgaL
#define request trlsts
#define HTTP_BIN ctfgbu
#define resultString sdtcbuh
#define JavaString xdctfin
#define get las
...


```

Resulting C/C++ Decompilation after obfuscation
```c++
...

    ghjRkl *xrdtcf = new ghjRkl(afh);
    jstring sdfgaL = xrdtcf->trlsts(afh, ghjRkl::ctfgbu);
    std::string sdtcbuh = xdctfin(afh, sdfgaL).las();

```

##Android Resources Decode

Encoded Android String Resources
```xml
<resources>
    <string name="app_name">Android Jni Helpers</string>
    <string name="api_security_token">dcfvhglasbiunpltsxrdtcfvygubhinjom</string>
</resources>


```

Insertion of getDecodedString(int) method into Context Classes (Application, Activity, etc.)
```java
String decodedString = getDecodedString(R.string.api_security_token);

```

##Easy Jni Replication of Android APIs
```bash
export className="org.json.JSONObject"
AndroidJniHelpers/bin/jni.bash $className

cat AndroidJniHelpers/bin/jni.files/generated/$className.jni

```

Example of generated Jni Helper result
```c++
org.json.JSONObject

********************************************************************************

org.json.JSONObject.jni was generated as a helper for JSONObject.java using the
bin/jni.bash script. The following code segments are C++ header and source code containing:

- getCanonicalName: The method that enables the relationship between C++ and Java.
- Java methods: org.json.JSONObject, org.json.JSONObject, length, remove, isNull, has, opt, optBoolean, optBoolean, optDouble, optDouble, optInt, optInt, optLong, optLong, optString, optString, optJSONArray, optJSONObject, keys, names, toString, quote, wrap

The source code can be copied into the respective JSONObject.h and
JSONObject.cpp files in a location of your choice. Finally, the last segment
contains an example of what these method calls would look like in your code.

********************************************************************************

#include "JniHelpers.h"

class JSONObject : public JavaClass {
    jobject thisObj;
  public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the JSONObject class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("org/json", JSONObject);
    } ...
```

##Easy Jni Replication of Personal APIs
```bash
export className="my.personal.Object"
export filePath="file/path/to/my/personal/Object"
AndroidJniHelpers/bin/jni.bash $className $filePath

cat AndroidJniHelpers/bin/jni.files/generated/$className.jni

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
