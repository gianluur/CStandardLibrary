#include "../utility/types.h"
#include "../utility/memory.h"
#include "../utility/error.h"
#include <stdio.h>

typedef struct ListNode {
  void* data;
  struct ListNode *next;
  struct ListNode *prev;
} ListNode;

typedef struct list {
  struct ListNode* head;
  struct ListNode* tail;
  uint64 size, element_size;
} list;

#define listCreate(type) \
  list_create(sizeof(type))

#define listFree(list) \
  list_free(list) 

#define listInsertStart(list, element) ({ \
  typeof(element) temp = element; \
  list_insert_start(list, &temp); \
})

#define listInsertEnd(list, element) ({ \
  typeof(element) temp = element; \
  list_insert_end(list, &temp); \
})

#define listInsert(list, element, position) ({ \
  typeof(element) temp = element; \
  list_insert(list, &temp, position); \
})

ListNode *list_create_node(const void *element, const uint64 element_size){
  ListNode *new_node = (ListNode*)memoryAllocate(sizeof(ListNode));
  if (!new_node && errorcode == MEMORY_FAILURE) {
    return NULL;
  }

  new_node->data = memoryAllocate(element_size);
  if (!new_node->data && errorcode == MEMORY_FAILURE) {
    memoryFree(new_node);
    return NULL;
  }
  memoryCopy(element, element_size, new_node->data);

  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
}

void list_free_node(ListNode *node){
  memoryFree(node->data);
  memoryFree(node->next);
  memoryFree(node->prev);
  memoryFree(node);
}

list *list_create(const uint64 element_size){
  list *new_list = (list*)memoryAllocate(sizeof(list));
  if (!new_list && errorcode == MEMORY_FAILURE){
    return NULL;
  }

  new_list->element_size = element_size;
  new_list->head = NULL;
  new_list->tail = NULL;
  new_list->size = 0;

  return new_list;
}

void list_free(list *list){
  ListNode *current = list->head;
  ListNode* next = NULL;
  while (current) {
    next = current->next;
    list_free_node(current);
    current = next;
  }
}

bool list_insert(list *list, const void *element, uint64 position){
  if (!list){
    errorcode = LIST_INVALID;
    return false;
  }

  if (position > list->size){
    errorcode = LIST_OUT_OF_BOUNDS;
    return false;
  }

  ListNode *new_node = list_create_node(element, list->element_size);
  if (!new_node && errorcode == MEMORY_FAILURE){
    return false;
  }

  ListNode *current = NULL;
  if (position <= list->size / 2) {
    current = list->head;
    while (current->next && position > 0) {
      current = current->next;
      position--;
    }
  } 
  else {
    current = list->tail;
    while (current->prev && position < list->size - 1) {
      current = current->prev;
      position++;
    }
  }

  new_node->next = current->next;
  new_node->prev = current;
  current->next = new_node;

  if (!new_node->next) {
    list->tail = new_node;
  } else {
    new_node->next->prev = new_node;
  }

  list->size++;
  return true;
}

bool list_insert_start(list* list, const void *element){
  if (!list){
    errorcode = LIST_INVALID;
    return false;
  }

  ListNode *new_node = list_create_node(element, list->element_size);
  if (!new_node && errorcode == MEMORY_FAILURE) {
    return false;
  }

  if (!list->head) {
    list->head = new_node;
    list->tail = list->head;
  }
  else {
    new_node->next = list->head;
    list->head = new_node;
  }
  list->size++;

  errorcode = LIST_SUCCESS;
  return true;
}

bool list_insert_end(list *list, const void *element){
  if (!list){
    errorcode = LIST_INVALID;
    return false;
  }

  ListNode *new_node = list_create_node(element, list->element_size);
  if (!new_node && errorcode == MEMORY_FAILURE){
    return false;
  }

  if (!list->tail){
    list->tail = new_node;
    list->head = new_node;
  }
  else {
    list->tail->next = new_node;
    new_node->prev = list->tail;
    list->tail = new_node;
  }

  list->size++;
  return LIST_SUCCESS;
}

int main(){
  list *l = listCreate(int);
  if (!l)
    printf("fuck1");
  else 
    printf("LISTA C'E");

  if(listInsertStart(l, 5)){
    printf("start: %d ", *(int*)l->head->data);
  }

  if (listInsert(l, 7, 1)){
    printf("mid: %d ", *(int*)l->tail->data);
  }

  if (listInsertEnd(l, 6)){
    printf("end: %d", *(int*)l->tail->data);
  }
  listFree(l);
    

}