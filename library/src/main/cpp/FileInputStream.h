//
// Created by donnell on 1/20/17.
//

#ifndef APPLICATIONRESOURCES_FILEINPUTSTREAM_H
#define APPLICATIONRESOURCES_FILEINPUTSTREAM_H

#include "JniHelpers.h"

class FileInputStream : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the FileInputStream class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("java/io", FileInputStream);
    }
    FileInputStream();

    FileInputStream(JNIEnv *env, jobject stream);

    void initialize(JNIEnv *env);

    void mapFields();

//    jobject write(JNIEnv *env, jobject bytes);

//    void close(JNIEnv *env);

    jobject thisObj;
};


#endif //APPLICATIONRESOURCES_FILEOUTPUTSTREAM_H
