#ifndef OS_H
  #define OS_H

  #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
    #define OS_WINDOWS    
    #if defined(_WIN64)
      #define WINDOWS_64
      #define OS_NAME "Windows 64-bit"

    #else
      #define WINDOWS_32
      #define OS_NAME "Windows 32-bit"
    #endif

  #elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MAC
    #define OS_NAME "MacOS"
    #include <TargetConditionals.h>
          
    #if TARGET_OS_IPHONE
      #define OS_IOS
    #endif

  #elif defined(__linux__) || defined(__gnu_linux__)
    #define OS_LINUX
    #define OS_NAME "Linux"

  #elif defined(__unix__) || defined(__unix)
    #if defined(__FreeBSD__)
      #define OS_FREEBSD
      #define OS_NAME "FreeBSD"
    #elif defined(__OpenBSD__)
      #define OS_OPENBSD
      #define OS_NAME "OpenBSD"
    #else
      #define OS_UNIX
      #define OS_NAME "Unix"
    #endif

  #else
    #define OS_UNKNOWN
    #define OS_NAME "Unknown"
  #endif
#endif
