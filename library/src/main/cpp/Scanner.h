//
// Created by Christopher Miller on 1/12/17.
//

#ifndef APPLICATIONRESOURCES_SCANNER_H
#define APPLICATIONRESOURCES_SCANNER_H

#include "JniHelpers.h"
#include "File.h"

class Scanner : public JavaClass {

    jobject thisObj;
public:
    /**
    * This facsimile of the Java method java.lang.Class.getCanonicalName() is used to maintain
    * the Jni Helper's relationship to the File class defined in Java.
    */
    const char *getCanonicalName() const {
        return MAKE_CANONICAL_NAME("java/util", Scanner);
    }

    /**
    * This constructor is the loader for the Java instance used to call all of its methods.
    * You can construct the File object how ever you need to here.
    */
    Scanner(JNIEnv *env, File file) : JavaClass(env)
    {
        initialize(env);

        thisObj = env->NewObject(_clazz, getMethod("<init>"), file.thisObj); // CALLS DEFAULT CONSTRUCTOR

        if (thisObj == NULL) {
            JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
                                                     "thisObj variable not intialized, all methods of this class need an the thisObj Java instance.");
        }
    }

    void initialize(JNIEnv *env);

    void mapFields();

    void useDelimiter(JNIEnv *env, std::string delimt);

    jstring next(JNIEnv *env);
};


#endif //APPLICATIONRESOURCES_FILE_H
