//
// Created by donnell on 1/20/17.
//

#include "FileOutputStream.h"

FileOutputStream::FileOutputStream() {}//: JavaClass() {}

/**
* Here you can construct the FileOutputStream object how ever you need to,
* as well as load signatures for the Java instance method calls.
*/
FileOutputStream::FileOutputStream(JNIEnv *env, const char* filePath) //: JavaClass(env)
{
//    initialize(env);

    file.open (filePath);

    LOG_INFO("filePath: %s\n", filePath);

}

//void FileOutputStream::initialize(JNIEnv *env)
//{
//    setClass(env);
//    cacheConstructor(env);
//
//    cacheSignature(env, "getChannel", "()Ljava/nio/channels/FileChannel;");
//
//    registerNativeMethods(env);
//}

//void FileOutputStream::mapFields()
//{
//    //mapField("encryptedString", kTypeString, &encryptedString);
//}

jobject FileOutputStream::write(JNIEnv *env, const char* content)
{
    if (file.is_open()) {

        int length_ = (int) strlen(content);
        LOG_INFO("Writing: %d characters\n", length_);

        file << content;

        LOG_INFO("Wrote %s to file\n", content);

    }
    return 0;
}

void FileOutputStream::close(JNIEnv *env)
{
    file.close();
}

