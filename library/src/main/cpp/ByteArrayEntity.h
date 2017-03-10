//
// Created by donnell on 1/18/17.
//

#ifndef APPLICATIONRESOURCES_BYTEARRAYENTITY_H
#define APPLICATIONRESOURCES_BYTEARRAYENTITY_H


#include "JniHelpers.h"

class ByteArrayEntity : public JavaClass {
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the ByteArrayEntity class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("org/apache/http/entity", ByteArrayEntity);
    }
    ByteArrayEntity();

    ByteArrayEntity(JNIEnv *env, jbyteArray pArray);

    void initialize(JNIEnv *env);

    void mapFields();

    jboolean isRepeatable(JNIEnv *env);

    // long getContentLength;

    jobject getContent(JNIEnv *env);

    jboolean isStreaming(JNIEnv *env);

};


#endif //APPLICATIONRESOURCES_BYTEARRAYENTITY_H
