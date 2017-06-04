//
// Created by donnell on 1/20/17.
//

#ifndef APPLICATIONRESOURCES_PREFERENCEMANAGER_H
#define APPLICATIONRESOURCES_PREFERENCEMANAGER_H

#include "JniHelpers.h"

class PreferenceManager : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the PreferenceManager class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("android/preference", PreferenceManager);
    }

    PreferenceManager(JNIEnv *env);

    void initialize(JNIEnv *env);

    void mapFields();

    static jobject getDefaultSharedPreferences(JNIEnv *env, jobject jobjectValue1);

};


#endif //APPLICATIONRESOURCES_PREFERENCEMANAGER_H
