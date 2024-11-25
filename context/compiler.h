#ifndef COMPILER_H
  #define COMPILER_H

  #if defined(_MSC_VER)
    #define COMPILER_MSVC
    #define COMPILER_NAME "MSVC"
    #define COMPILER_VERSION _MSC_VER

  #elif defined(__clang__)
    #define COMPILER_CLANG
    #define COMPILER_NAME "Clang"
    #define COMPILER_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)

  #elif defined(__GNUC__)
    #define COMPILER_GCC
    #define COMPILER_NAME "GCC"
    #define COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

  #else
    #define COMPILER_UNKNOWN
    #define COMPILER_NAME "Unknown"
    #define COMPILER_VERSION 0
  #endif
#endif