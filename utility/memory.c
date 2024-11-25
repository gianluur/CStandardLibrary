#include "memory.h"

#ifdef OS_LINUX
void* allocate(const uint64 bytes){
  void *heap_memory = sbrk(0);
  if (sbrk(bytes) == (void*)-1)
    return nullptr; //add print statement
  return heap_memory;
}
#endif

int main(){
  struct Scroto {
    int x;
    int* y;
    char z;
  };

  struct Scroto* ptr = (struct Scroto*)allocate(sizeof(struct Scroto));

  if (!ptr){
    printf("Eh vabbe'");
  }
  printf("POOOOOOOOOOOOOOOH %p", ptr);
}