//
// Created by donnell on 1/20/17.
//

#ifndef APPLICATIONRESOURCES_FILEOUTPUTSTREAM_H
#define APPLICATIONRESOURCES_FILEOUTPUTSTREAM_H

#include "JniHelpers.h"

class FileOutputStream : public JavaClass {
    jobject thisObj;
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the FileOutputStream class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("java/io", FileOutputStream);
    }
    FileOutputStream();

    FileOutputStream(JNIEnv *env, jobject stream);

    void initialize(JNIEnv *env);

    void mapFields();

    jobject write(JNIEnv *env, jobject bytes);

    void close(JNIEnv *env);
};


#endif //APPLICATIONRESOURCES_FILEOUTPUTSTREAM_H
