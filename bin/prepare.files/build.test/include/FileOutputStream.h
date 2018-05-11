//
// Created by donnell on 1/20/17.
//

#ifndef APPLICATIONRESOURCES_FILEOUTPUTSTREAM_H
#define APPLICATIONRESOURCES_FILEOUTPUTSTREAM_H

#include <fstream>
#include "JniHelpers.h"

using namespace std;

class FileOutputStream {//: public JavaClass {
//    jobject thisObj;
public:

    ofstream file;
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the FileOutputStream class defined in Java.
    */
//    const char *getCanonicalName() const {
//        return MAKE_CANONICAL_NAME("java/io", FileOutputStream);
//    }
    FileOutputStream();

    FileOutputStream(JNIEnv *env, const char* path);

//    void initialize(JNIEnv *env);
//
//    void mapFields();

    jobject write(JNIEnv *env, const char* bytes);

    void close(JNIEnv *env);
};


#endif //APPLICATIONRESOURCES_FILEOUTPUTSTREAM_H
