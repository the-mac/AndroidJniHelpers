//
// Created by donnell on 1/20/17.
//

#ifndef APPLICATIONRESOURCES_SHAREDPREFERENCES_H
#define APPLICATIONRESOURCES_SHAREDPREFERENCES_H


#include <JniHelpers.h>

class SharedPreferences : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the SharedPreferences class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("android/content", SharedPreferences);
    }
    SharedPreferences();

    SharedPreferences(JNIEnv *env, jobject context);

    void initialize(JNIEnv *env);

    void mapFields();

    jstring getString(JNIEnv *env, jstring jstringValue1, jstring jstringValue2);

    jboolean getBoolean(JNIEnv *env, jstring jstringValue1, jboolean jbooleanValue2);

    jboolean contains(JNIEnv *env, jstring jstringValue1);

    jobject edit(JNIEnv *env);

    class Editor : public JavaClass {
        jobject thisObj;
    public:
        /**
        * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
        * the Jni Helper's relationship to the Editor class defined in Java.
        */
        const char *getCanonicalName() const {
            return MAKE_CANONICAL_NAME_INNER_CLASS("android/content/SharedPreferences", Editor);
        }
        Editor();

        Editor(JNIEnv *env, jobject editor);

        void initialize(JNIEnv *env);

        void mapFields();

        jobject putString(JNIEnv *env, jstring jstringValue1, jstring jstringValue2);

        jobject putBoolean(JNIEnv *env, jstring jstringValue1, jboolean jbooleanValue2);

        jobject remove(JNIEnv *env, jstring jstringValue1);

        jobject clear(JNIEnv *env);

        jboolean commit(JNIEnv *env);

    };

};



#endif //APPLICATIONRESOURCES_SHAREDPREFERENCES_H
