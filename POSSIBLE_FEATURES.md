#Possible Features:
##Cache Static Methods
Will making the jclass property static make static methods easier?

Try testing whether JavaClassTest passes with jclass _clazz and isInitialized declared static!

If the test passes create getClass method to be used, and test for TestObject & JavaClassTest
- http://en.cppreference.com/w/cpp/language/static
- https://stackoverflow.com/questions/34222703/how-to-override-static-method-of-template-class-in-derived-class

What getStaticMethod usage would look like:
```c++

    const char* AndroidJniApp::getSignature(const char* functionName)
    {
        return "us/the/mac/android/jni/helpers/AndroidJniApp." functionName;
    }

    jobject AndroidJniApp::Instance(JNIEnv *env)
    {
        jmethodID method = JavaClass::getStaticMethod(AndroidJniApp::getSignature(__FUNCTION__))
        jobject result = env->CallStaticObjectMethod(_class, method);
        JavaExceptionUtils::checkException(env);
        return result;
    }

```

What getStaticMethod implmentation/usage would look like:
```c++

        jmethodID JavaClass::getStaticMethod(const char *method_name) const {
            if (!isInitialized()) {
                JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
                                                         kTypeIllegalStateException,
                                                         "Cannot call getMethod without class info (forgot to merge?)");
                return NULL;
            }

            const std::string key(method_name);
            MethodMap::const_iterator mapFindIter = _static_methods->find(key);
            if (mapFindIter == _static_methods->end()) {
                JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
                                                         kTypeIllegalArgumentException,
                                                         "Static method '%s' is not cached in class '%s'",
                                                         method_name, getCanonicalName());
                return NULL;
            }

            return mapFindIter->second;
        }

```

What cacheStaticSignature implmentation/usage would look like:
```c++

    cacheStaticSignature(env, "staticMethod", "()V");

    void JavaClass::cacheStaticSignature(JNIEnv *env, const char *method_name, const char *signature) {
        LOG_DEBUG("Caching static method '%s' in class '%s'", method_name, getSimpleName());
        if (!isInitialized()) {
            JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                     "Attempt to call cacheMethod without having set class info");
            return;
        }

        jmethodID method = env->GetStaticMethodID(_clazz_global.get(), method_name, signature);
        JavaExceptionUtils::checkException(env);
        if (method != NULL) {
            _static_methods_global[getCanonicalName() "." method_name] = method;
        } else {
            JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaClass(NoSuchMethodError),
                                                     "Method '%s' (signature: %s) not found on class '%s'",
                                                     method_name, signature,
                                                     getCanonicalName());
        }
    }

    ...

    jobject response = AndroidJniApp::Instance(env);

```


##Header Obfuscation (Inserted into main c/cpp file)

What file looks like before obfuscation:
```c++

//#include "obfuscate.h"
...

    Network *network = new Network(env);
    jstring response = network->request(env, Network::HTTP_BIN);
    std::string resultString = JavaString(env, response).get();

```

Example generation of Obfuscation Header (obfuscate.h)
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

Resulting C/C++ decompilation after obfuscation
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
    <string name="website_passphrase">dcfvhglasbiunpltsxrdtcfvygubhinjom</string>
</resources>
```

Insertion of getDecodedString(int) method into Context Classes (Application, Activity, etc.)
```java
String decodedString = getDecodedString(R.string.website_passphrase);

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
  public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the JSONObject class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("org/json", JSONObject);
    }
    ...
```

##Easy Jni Replication of Personal APIs
```bash
export className="my.personal.Object"
export filePath="file/path/to/my/personal/Object"
AndroidJniHelpers/bin/jni.bash $className $filePath

cat AndroidJniHelpers/bin/jni.files/generated/$className.jni

```