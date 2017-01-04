# AndroidJniHelpers
Tools for writing secure Android JNI code

The Android Jni Helpers library you can hide important details of your app from
basic apk decompilation, and harden your app beyond [basic code shrinking](https://developer.android.com/studio/build/shrink-code.html)
from [proguard obfuscation](https://www.guardsquare.com/en/proguard). The project is based upon [Spotify's Jni Helpers](https://github.com/the-mac/JniHelpers.git)
library. Here are some of our current features:

**Native String Decoding**
```java

    DecodedString object = DecodedString.getInstance();
    object.encodedString = "Up cf ps opu up cf, Uibu jt uif rvftujpo";

    String decodedString = object.decode();
    assertEquals("To be or not to be, That is the question", decodedString);

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

