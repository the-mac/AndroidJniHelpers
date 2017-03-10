#ifndef __TestConstants_h__
#define __TestConstants_h__

/**
 * Common values for tests which pass objects through JNI.
 * NB: This file is GENERATED. DO NOT EDIT.
 */

#define TEST_URL "https://httpbin.org/post"
#define TEST_REQUEST "{ \"parameter\": \"parameterValue\" }"
#define TEST_PARAMETER "parameterValue"
#define TEST_RESULT "{ \"args\": {}, \"data\": \"\", \"files\": {}, \"form\": {}, \"headers\": { \"Accept\": \": application/json\", \"Content-Length\": \"0\", \"Content-Type\": \"application/json\", \"Host\": \"httpbin.org\", \"User-Agent\": \"Apache-HttpClient/UNAVAILABLE (java 1.4)\" }, \"json\": null, \"origin\": \"172.58.153.6\", \"url\": \"https://httpbin.org/post\" } "
#define TEST_HEADERS "application/json"
#define TEST_ENCRYPTED "Up cf ps opu up cf, Uibu jt uif rvftujpo"
#define TEST_ENCRYPTION_KEY "FHppc7om5dCkJGzhHYc5ug==\n"
#define TEST_ENCRYPTED_RESOURCE "HiYJ+OroGm0hwPv5hz9V0g==:Vm7lfkeQgs0p4G//PFjq7C9eePpwq3PU4mnIDLY2yQWmFITN2JeE7UfuQUb2sdqZ"
#define TEST_DECRYPT "To be or not to be, That is the question"
#define TEST_STRING "hello, is it me you're looking for?"
#define TEST_NULL_CHAR "\0"
#define TEST_STRING_WITH_NULL_CHAR "hello \0 world"
#define TEST_UTF16_STRING u"UTF16 hello: \u2018."
#define TEST_UTF8_STRING u8"a \u0915\u093E\u091A\u0902 \u00F6 1 2 3"
#define TEST_INTEGER 42
#define TEST_SHORT 777
#define TEST_BOOLEAN true
#define TEST_FLOAT 3.14159f
#define TEST_FLOAT_TOLERANCE 0.01
#define TEST_DOUBLE 2.71828
#define TEST_BYTE 72
#define TEST_CHAR L'ö'

#endif // __TestConstants_h__
