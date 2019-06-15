# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(echo LANGUAGES C CXX)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
include_directories(    ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/include/opencl
                        ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/include/opencl/include
                        ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/include/opencl/include/CL
                        ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp
                        ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/include/opencl/opencl22)

set(CMAKE_VERBOSE_MAKEFILE on)
set(libs "${CMAKE_SOURCE_DIR}/src/main/jniLibs")
#--------------------------------------------------- import ---------------------------------------------------#
#add_library(libOpenCL SHARED IMPORTED )
#set_target_properties(libOpenCL PROPERTIES
#        IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libOpenCL.so")
#message(ANDROID_ABwwwwwwwwwwwwI ,${ANDROID_ABI})



if(${ANDROID_ABI} STREQUAL arm64-v8a )
    add_library(libcutils SHARED IMPORTED )
    set_target_properties(libcutils PROPERTIES
    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libcutils.so")

    add_library(libbase SHARED IMPORTED )
    set_target_properties(libbase PROPERTIES
    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libbase.so")

    add_library(libcpp SHARED IMPORTED )
    set_target_properties(libcpp PROPERTIES
    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libc++.so")
endif()



add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp
        src/main/cpp/OpenCLWrapper.h
        src/main/cpp/OpenCLWrapper.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.


if(ANDROID_ABI MATCHES "^armeabi(-v7a)?$")
    target_link_libraries( # Specifies the target library.
            native-lib
            ${log-lib})
elseif(ANDROID_ABI STREQUAL arm64-v8a)
    target_link_libraries( # Specifies the target library.
            native-lib libbase  libcpp libcutils libOpenCL
            ${log-lib})
endif()