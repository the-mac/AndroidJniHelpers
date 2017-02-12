//
// Created by donnell on 1/20/17.
//

#ifndef APPLICATIONRESOURCES_ANDROIDJNIAPP_H
#define APPLICATIONRESOURCES_ANDROIDJNIAPP_H

#include "JniHelpers.h"

class AndroidJniApp : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the AndroidJniApp class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("us/the/mac/android/jni/helpers", AndroidJniApp);
    }
    AndroidJniApp();

    AndroidJniApp(JNIEnv *env);

    AndroidJniApp(JNIEnv *env, jobject context);

    void initialize(JNIEnv *env);

    void mapFields();

    static jobject Instance(JNIEnv *env);

    jstring getFilesDir(JNIEnv *env);

    jobject openFileOutput(JNIEnv *env, jstring path, jint mode);

    jstring decryptStringNative(JNIEnv *env, jint resource);

    static jstring decryptString(JNIEnv *env, jobject java_this, jint resource);

    static JavaClass* getPersistedInstance(JNIEnv *env, jobject javaThis);

    jobject thisObj;

    static const int MODE_PRIVATE = 0;

};



#endif //APPLICATIONRESOURCES_REELAPPLICATION_H
