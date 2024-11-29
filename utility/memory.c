#include "memory.h"
#include "types.h"
#include "error.h"

#include <stdio.h>
#include <unistd.h>

//I know that with bitwise operations some operations could be faster but i hate non readable code

#ifdef OS_LINUX
#define ALIGNMENT sizeof(void*) //helps keeping the memory as compact as possible, this increases perfromances as well on modern CPUs

//Rounds everything by 8 byte
//ALIGN(1) = 8, ALIGN(7) = 8, 
//ALIGN(9) = 16, ALIGN(17) = 24
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1)) 

typedef struct MemoryBlock {
  int64 size;
  bool free;
  struct MemoryBlock* next;
} MemoryBlock;

MemoryBlock *blockList = NULL;

#define BLOCK_SIZE sizeof(MemoryBlock)
#define MIN_BLOCK_SIZE (BLOCK_SIZE + sizeof(int32))

MemoryBlock *findFreeSpace(MemoryBlock **last, const int64 size){
  MemoryBlock *current = blockList;
  MemoryBlock *smaller = NULL;
  while (current){
    if (current->free && current->size - size >= size)
      if (!smaller || current->size < smaller->size)
        smaller = current;
    
    *last = current;
    current = current->next;
  }
  return smaller;
}

MemoryBlock *requestSpace(MemoryBlock *last, const int64 size){
  MemoryBlock *block = (MemoryBlock*)sbrk(0);
  if (block == (void*)-1){
    errorcode = MEMORY_FAILURE;
    return NULL;
  }

  if (sbrk(ALIGN(BLOCK_SIZE + size)) == (void*)-1){
    errorcode = MEMORY_FAILURE;
    return NULL;
  }

  block->size = size;
  block->free = false;
  block->next = NULL;

  if (last)
    last->next = block;
  
  errorcode = MEMORY_SUCCESS;
  return block;
}

void splitBlock(MemoryBlock *block, const uint64 size){
  MemoryBlock *newBlock = (MemoryBlock*)((int8*)block + BLOCK_SIZE + size); //calculates the address next to the given block
  newBlock->size = block->size - size - BLOCK_SIZE;
  newBlock->free = true;
  newBlock->next = block->next;

  block->size = size;
  block->next = newBlock;
}

void *memoryAllocate(const uint64 size){
  if (size <= 0 || size > MAXUINT64 - BLOCK_SIZE){ //checks if the size given is invalid
    errorcode = MEMORY_FAILURE;
    return NULL;
  }
  
  MemoryBlock *block;
  if (!blockList){ //checks if there are no memory block allocated, if so it request some memory from the OS
    block = requestSpace(NULL, size);

    if (!block){  // if the block is null, the request to the os failed
      errorcode = MEMORY_FAILURE;
      return NULL;
    }

    blockList = block; // if the request is successful, assign it to the block list
  }

  else {  // if there's already some memory blocks allocated, checks whetere they are free
    MemoryBlock *last = blockList;
    block = findFreeSpace(&last, size);

    if (!block) {  //if no memory block is currently free then it request some memory
      block = requestSpace(last, size);
      
      if (block == NULL) {
        errorcode = MEMORY_FAILURE;
        return NULL;
      }
    }
    else { // if there's some free memory, it tries to split the block to the size required to leave as much free space as possible
      if (block->size >= ALIGN(BLOCK_SIZE + size))
        splitBlock(block, size);
      block->free = false; // makes that memory block not free anymore because we're gonna return it soon
    }
  }

  errorcode = MEMORY_SUCCESS;
  return (block + 1); // returns the block of memory avoiding to return the block metadat as well. This is equal as block + sizeof(MemoryBlock)
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

void zeroAllBytes(void *ptr, const uint64 size){
  int8 *byte_ptr = (int8*)ptr;
  for (uint64 i = 0; i < size; ++i)
    byte_ptr[i] = 0;
}

void memoryFree(void *ptr){
  if (!ptr) //TODO: Add log here
    return;

  MemoryBlock *block = (MemoryBlock*)ptr - 1; //ptr points to the memory block, we go a sizeof(MemoryBlock) back in order to get the metadata
  block->free = true;

  zeroAllBytes(ptr, block->size);
  coalesceBlocks(); //tries to adjacent memory block is a bigger chunck to avoid memory fragmentation
}

void memoryCopy(const void *from, const uint64 from_size, const void *to){
  const int8 *byte_from = (int8*)from;
  int8 *byte_to = (int8*)to;

  for (uint64 i=0; i<from_size; ++i)
    byte_to[i] = byte_from[i];
}

void *memoryReallocate(void* ptr, const uint64 new_size){
  if (!ptr) //TODO: if this needs to be changed
    return NULL;
  
  MemoryBlock *block = (MemoryBlock*)ptr -1;

  if (new_size < block->size){ //if the reallocation wants to shrink the needed memory
    if (block->size - new_size > BLOCK_SIZE + MIN_BLOCK_SIZE) //try to split the block if the reduction is significant
      splitBlock(block, new_size);
    errorcode = MEMORY_SUCCESS;
    return ptr; //if not possible just return the ptr
  }

  else if (block->next && block->next->free){ //if the reallocation needs more memory, check if the next block is free
    MemoryBlock* next = block->next;
    const uint64 totalMemoryAvailable = block->size + next->size + BLOCK_SIZE; //sum the size of the 2 blocks
    if (totalMemoryAvailable > new_size){ // if it's greater than new size needed then merge the 2 block
      block->size += next->size + BLOCK_SIZE; //increase size of this block with the one of the next block 
      block->next = next->next; //make it point to next block (since they've been merged it's 2 block ahead of the current, including the now merged)

      if (block->size > new_size + BLOCK_SIZE + MIN_BLOCK_SIZE) //if the size is big enough split the block to the use the least memory free possible
        splitBlock(block, new_size);
    }
    errorcode = MEMORY_SUCCESS;
    return ptr;
  }

  else { // reallocate everything somewhere else, if no previous optimizaion worked
    void* new_ptr = memoryAllocate(new_size);
    if (!new_ptr){
      errorcode = MEMORY_FAILURE;
      return NULL;
    }

    memoryCopy(ptr, block->size, new_ptr);
    memoryFree(ptr);

    errorcode = MEMORY_SUCCESS;
    return new_ptr;
  }
}

void *memoryCalloc(const uint64 nitems, const uint64 item_size){
  const uint64 size = nitems * item_size;
  if (size > MAXUINT64) { //add log
    errorcode = MEMORY_FAILURE;
    return NULL;
  } 

  void *ptr = memoryAllocate(nitems * item_size);
  if (!ptr){
    errorcode = MEMORY_FAILURE;
    return NULL;
  }
  zeroAllBytes(ptr, size);


  errorcode = MEMORY_SUCCESS;
  return ptr;
}

#endif

// int main(){
//   int* arr = (int*)memoryAllocate(3 * sizeof(int));
//   if (!arr)
//     printf("fuck");
//   else {
//     arr[0] = 1;
//     arr[1] = 2;
//     printf("YAY: %p, %p, %p, %p %d\n", arr, &arr[1], &arr[2], &arr[3], memoryStatus);
//   }

//   memoryReallocate(arr, 5);
//   arr[4] = 4;
//   arr[5] = 5;
//   printf("Realloc: %p, %p\n", &arr[4], &arr[5]);

//   int *arr2 = (int*)memoryCalloc(3, sizeof(int));
//   printf("Calloc: %p: %d, %p:%d\n", &arr2[0], arr2[0], &arr2[1], arr2[1]);

//   memoryFree(arr);
//   printf("%d\n", arr[0]);
// }