#include "memory.h"
#include "types.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//I know that with bitwise operations some operations could be faster but i hate non readable code

#define HEAP_FAILURE NULL

#ifdef OS_LINUX
#define ALIGNMENT 8 //Linux requires 8-byte alignment between memory block and it's metadata

//Rounds everything by 8 byte
//ALIGN(1) = 8, ALIGN(7) = 8, 
//ALIGN(9) = 16, ALIGN(17) = 24
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1)) 

typedef struct MemoryBlock {
  int64 size;
  bool free;
  struct MemoryBlock* next;
} MemoryBlock;

#define BLOCK_SIZE sizeof(MemoryBlock)
#define MIN_BLOCK_SIZE (BLOCK_SIZE + sizeof(int32))

MemoryBlock *blockList = NULL;

MemoryBlock *findFreeSpace(MemoryBlock **last, const int64 size){
  MemoryBlock* current = blockList;
  while (current && !(current->size >= size && current->free)){
    *last = current;
    current = current->next;
  }
  return current; //could be null if block isn't found
}

MemoryBlock *requestSpace(MemoryBlock *last, const int64 size){
  MemoryBlock *block = (MemoryBlock*)sbrk(0);
  if (block == (void*)-1)
    return HEAP_FAILURE;

  if (sbrk(ALIGN(BLOCK_SIZE + size)) == (void*)-1)
    return HEAP_FAILURE;

  block->size = size;
  block->free = false;
  block->next = NULL;

  if (last)
    last->next = block;

  return block;
}

void splitBlock(MemoryBlock *block, const uint64 size){
  if (block->size <= ALIGN(BLOCK_SIZE + size))
    return;

  MemoryBlock *newBlock = (MemoryBlock*)((int8*)block + BLOCK_SIZE + size); //calculates the address next to the given block
  newBlock->size = block->size - size - BLOCK_SIZE;
  newBlock->free = true;
  newBlock->next = block->next;

  block->size = size;
  block->next = newBlock;
}

void *memoryAllocate(const uint64 size){
  if (size <= 0 || size > MAXUINT64 - BLOCK_SIZE)
    return NULL;
  
  MemoryBlock *block;
  if (blockList == NULL){
    block = requestSpace(NULL, size);
    if (block == HEAP_FAILURE)
      return HEAP_FAILURE;
    blockList = block;
  }
  else {
    MemoryBlock *last = blockList;
    block = findFreeSpace(&last, size);
    if (block == NULL){
      block = requestSpace(last, size);
      if (block == HEAP_FAILURE)
        return HEAP_FAILURE;
    }
    else{
      splitBlock(block, size);
      block->free = false; //need to split the block if possible
    }
  }
  return (block + 1);
}

void coalesceBlocks() {
  MemoryBlock *current = blockList;
  
  while (current && current->next) {
    if (current->free && current->next->free) { // Check if current and next blocks are free and can be merged
      current->size += BLOCK_SIZE + current->next->size;
      current->next = current->next->next;
    } 
    else 
      current = current->next;
  }
}

void memoryFree(void *ptr){
  if (ptr == NULL) //TODO: Add log here
    return;

  MemoryBlock *block = (MemoryBlock*)ptr - 1; //to get block metadata
  block->free = true;

  coalesceBlocks();
}

void zeroAllBytes(void *ptr, const uint64 size){
  int8 *byte_ptr = (int8*)ptr;
  for (uint64 i = 0; i < size; ++i)
    byte_ptr[i] = 0;
}

void memoryCopy(const void *from, const uint64 from_size, const void *to){
  const int8 *byte_from = (int8*)from;
  int8 *byte_to = (int8*)to;

  for (uint64 i=0; i<from_size; ++i)
    byte_to[i] = byte_from[i];
}

void *memoryReallocate(void* ptr, const uint64 size){
  if (!ptr)
    return NULL;
  
  MemoryBlock *block = (MemoryBlock*)ptr -1;
  if (block->size >= size)
    return block;

  void* new_ptr = memoryAllocate(size);
  if (!new_ptr)
    return HEAP_FAILURE;
  memoryCopy(ptr, block->size, new_ptr);
  memoryFree(ptr);

  return new_ptr;
}

void *memoryCalloc(const uint64 nitems, const uint64 item_size){
  const uint64 size = nitems * item_size;
  if (size > MAXUINT64) //add log
    return NULL;

  void *ptr = memoryAllocate(nitems * item_size);
  if (!ptr)
    return HEAP_FAILURE;

  zeroAllBytes(ptr, size);
  return ptr;
}

#endif

int main(){
  int* arr = (int*)memoryAllocate(3 * sizeof(int));
  if (!arr)
    printf("fuck");
  else {
    arr[1] = 1;
    arr[2] = 2;
    arr[0] = 3;
    printf("YAY: %p, %p, %p, %p", arr, &arr[1], &arr[2], &arr[3]);
  }
  memoryFree(arr);
  printf("%p", &arr[0]);
}



