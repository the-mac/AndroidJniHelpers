//
// Created by Christopher Miller on 1/12/17.
//

#ifndef APPLICATIONRESOURCES_FILE_H
#define APPLICATIONRESOURCES_FILE_H

#include "JniHelpers.h"

class File : public JavaClass {

public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the File class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("java/io", File);
    }

    /**
    * This constructor is the loader for the Java instance used to call all of its methods.
    * You can construct the File object how ever you need to here.
    */
    File(JNIEnv *env, jstring filePath) : JavaClass(env)
    {
        initialize(env);

        thisObj = env->NewObject(_clazz, getMethod("<init>"), filePath); // CALLS DEFAULT CONSTRUCTOR

        if (thisObj == NULL) {
            JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                     "thisObj variable not intialized, all methods of this class need an the thisObj Java instance.");
        }
    }

    /**
    * This constructor is the loader for the Java instance used to call all of its methods.
    * You can construct the File object how ever you need to here.
    */
    File(JNIEnv *env, jstring filePathStart, jstring filePathEnd) : JavaClass(env)
    {
        initialize(env);

        thisObj = env->NewObject(_clazz, getMethod("<init>"), filePathStart, filePathEnd); // CALLS DEFAULT CONSTRUCTOR

        if (thisObj == NULL) {
            JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                     "thisObj variable not intialized, all methods of this class need an the thisObj Java instance.");
        }
    }

    void initialize(JNIEnv *env);

    void mapFields();

    jstring getAbsolutePath(JNIEnv *env);

    jboolean exists(JNIEnv *env);

    jboolean deleteNative(JNIEnv *env);

};


#endif //APPLICATIONRESOURCES_FILE_H
