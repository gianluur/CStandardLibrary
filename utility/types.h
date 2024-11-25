#ifndef TYPES_H
  #define TYPES_H

  typedef unsigned char uint8;
  typedef unsigned short uint16;
  typedef unsigned int uint32;
  typedef unsigned long long uint64;

  typedef signed char int8;
  typedef signed short int16;
  typedef signed int int32;
  typedef signed long long int64;

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

#ifndef __cplusplus
    typedef int8 bool;
  #endif

  #define true 1
  #define false 0
  #define nullptr ((void*)0)

#endif


