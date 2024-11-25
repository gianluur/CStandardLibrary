#ifndef ARCHITECTURE_H
  #define ARCHITECTURE_H

  #if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)
    #define ARCHITECTURE_H_X64
    #define ARCHITECTURE_H_NAME "x64"

  #elif defined(__i386__) || defined(_M_IX86)
    #define ARCHITECTURE_H_X86
    #define ARCHITECTURE_H_NAME "x86"

  #elif defined(__arm__) || defined(_M_ARM)
    #define ARCHITECTURE_H_ARM
    #define ARCHITECTURE_H_NAME "ARM"

  #elif defined(__aARCHITECTURE_H64__)
    #define ARCHITECTURE_H_ARM64
    #define ARCHITECTURE_H_NAME "ARM64"

  #else
    #define ARCHITECTURE_H_UNKNOWN
    #define ARCHITECTURE_H_NAME "Unknown"
  #endif
#endif
