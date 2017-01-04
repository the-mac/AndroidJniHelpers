cmake_minimum_required(VERSION 3.4.1)

set(library_JniHelpers_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../JniHelpers/src/main/cpp)
set(library_JniHelpersTest_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../JniHelpers/src/test/cpp)

file(GLOB libJniHelpers_SOURCES ${library_JniHelpers_DIR}/*.cpp)
file(GLOB libJniHelpers_HEADERS ${library_JniHelpers_DIR}/*.h)

file(GLOB libJniHelpersTest_SOURCES ${library_JniHelpersTest_DIR}/*.cpp)
file(GLOB libJniHelpersTest_HEADERS ${library_JniHelpersTest_DIR}/*.h)

link_directories(${JNI_LIBRARIES})
add_library(JniHelpers STATIC ${libJniHelpers_SOURCES})
add_library(JniHelpersTest STATIC ${libJniHelpers_SOURCES} ${libJniHelpersTest_SOURCES})
include_directories(${library_JniHelpers_DIR} ${library_JniHelpersTest_DIR})


if(${ANDROID_TESTING})
    set(library_NAME "test-libs")
else()
    set(library_NAME "native-lib")
endif()

add_library( # Sets the name of the library.
             ${library_NAME}

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/cpp/${library_NAME}.cpp )

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


# Only adding JniHelpersTest library if testing
if(${ANDROID_TESTING})
    target_link_libraries(${library_NAME} JniHelpers JniHelpersTest ${log-lib} )
else()
    target_link_libraries(${library_NAME} JniHelpers ${log-lib} )
endif()