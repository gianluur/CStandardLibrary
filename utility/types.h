#include "../context/compiler.h"

#ifndef TYPES_H
  #define TYPES_H

  //======BASIC TYPES======//

  //====UNSIGNED====//
  typedef unsigned char uint8;
  typedef unsigned short uint16;
  typedef unsigned int uint32;
  typedef unsigned long long uint64;

  //====SIGNED====//
  typedef signed char int8;
  typedef signed short int16;
  typedef signed int int32;
  typedef signed long long int64;

  //====MISC====//
  #ifndef __cplusplus
    typedef int8 bool;
  #endif

  #define true 1
  #define false 0

  #define NULL ((void *)0)

  #define OVERFLOW NULL
  #define UNDERFLOW NULL

  #if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
    #define typeof(type) __typeof__(type)
  #elif defined(COMPILER_MSVC)
    #define typeof(type) decltype(type)
  #endif


  //======MAX & MIN VALUES======//
  #define MAXUINT8 255U
  #define MAXUINT16 65535U
  #define MAXUINT32 4294967295U
  #define MAXUINT64 18446744073709551615ULL

  #define MAXINT8 127
  #define MAXINT16 32767
  #define MAXINT32 2147483647
  #define MAXINT64 9223372036854775807LL

  #define MININT8 -128
  #define MININT16 -32768
  #define MININT32 -2147483648
  #define MININT64 -9223372036854775807LL

  //======DATA STRUCTURES======//
  typedef struct String String;
  typedef struct Vector Vector;
  
  typedef struct ListNode ListNode;
  typedef struct List List;

  typedef struct StackNode StackNode;
  typedef struct Stack Stack;

  typedef struct QueueNode QueueNode;
  typedef struct Queue Queue;

  typedef struct MapBucket MapBucket;
  typedef struct Map Map;

#endif