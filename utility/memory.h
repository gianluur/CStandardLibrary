#include "types.h"
#include "../context/os.h"
#include <stdio.h>
#include <unistd.h>

void *memoryAllocate(const uint64 size);
void *memoryReallocate(void* ptr, const uint64 size);
void *memoryCalloc(const uint64 nitems, const uint64 item_size);
void memoryFree(void *ptr);