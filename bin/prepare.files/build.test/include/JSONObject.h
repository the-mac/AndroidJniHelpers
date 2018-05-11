//
// Created by Christopher Miller on 1/20/17.
//

#ifndef ANDROIDJNI_JSONOBJECT_H
#define ANDROIDJNI_JSONOBJECT_H

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

    JSONObject();

    JSONObject(JNIEnv *env, std::string json);

    JSONObject(JNIEnv *env, jstring jsonObject);

    JSONObject(JNIEnv *env, jobject jsonObject);

    void initialize(JNIEnv *env);

    void mapFields();

    jboolean has(JNIEnv *env, std::string key);

    jobject getJSONObject(JNIEnv *env, std::string stringKey);

    jstring getString(JNIEnv *env, std::string stringKey);

    jboolean getBoolean(JNIEnv *env, std::string stringKey);

};



#endif //ANDROIDJNI_JSONOBJECT_H
