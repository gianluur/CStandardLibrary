#include "types.h"
#include "../context/os.h"
#include <stdio.h>
#include <unistd.h>


void *memoryAllocate(const uint64 size);
void *memoryReallocate(void* ptr, const uint64 size);
void *memoryCalloc(const uint64 nitems, const uint64 item_size);
void memoryCopy(const void *from, const uint64 from_size, const void *to);
void memoryFree(void *ptr);