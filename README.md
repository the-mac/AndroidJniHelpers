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

    jstring decodedString = object->decode(env, object->toJavaObject(env));
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

