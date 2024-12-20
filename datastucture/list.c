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

ListNode *list_create_node(const void *element, const uint64 element_size) {
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

void list_free_node(ListNode *node) {
  if (!node)
    return;

  memoryFree(node->data);
  memoryFree(node);
}

list *list_create(const uint64 element_size) {
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

void list_free(list *list) {
  ListNode *current = list->head;
  ListNode* next = NULL;
  while (current) {
    next = current->next;
    list_free_node(current);
    current = next;
  }
}

void *list_start(list *list) {
  return list->head->data;
}

void *list_end(list *list) {
  return list->tail->data;
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

  if (!list->tail) {
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

bool list_insert(list *list, const void *element, uint64 position) {
  if (!list || position > list->size) {
    errorcode = LIST_INVALID;
    return false;
  }

  if (position == 0) { // Insert at the start
    return list_insert_start(list, element);
  } else if (position == list->size) { // Insert at the end
    return list_insert_end(list, element);
  }

  ListNode *new_node = list_create_node(element, list->element_size);
  if (!new_node) {
    return false;
  }

  ListNode *current = list->head;
  for (uint64 i = 0; i < position; i++) {
    current = current->next;
  }

  new_node->next = current;
  new_node->prev = current->prev;

  current->prev->next = new_node;
  current->prev = new_node;

  list->size++;
  return LIST_SUCCESS;
}

bool list_remove(list *list, uint64 position){
  if (!list) {
    errorcode = LIST_INVALID;
    return false;
  }

  if (position > list->size - 1) {
    errorcode = LIST_OUT_OF_BOUNDS;
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
    position = list->size - position - 1;
    while (current->prev && position > 0) {
      current = current->prev;
      position--;
    }
  }

  if (current->prev){
    current->prev->next = current->next;
  }
  else {
    list->head = current->next;
  }

  if (current->next){
    current->next->prev = current->prev;
  }
  else {
    list->tail = current->prev;
  }

  list_free_node(current);
  list->size--;
  return true;
}

bool list_remove_start(list *list) {
  if (!list || !list->head) {
    errorcode = LIST_INVALID;
    return false;
  }

  ListNode *to_free = list->head;

  if (!list->head->next) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    list->head = list->head->next;
    list->head->prev = NULL;
  }

  list_free_node(to_free);
  list->size--;

  return LIST_SUCCESS;
}

bool list_remove_end(list *list) {
  if (!list || !list->tail) {
    errorcode = LIST_INVALID;
    return false;
  }

  ListNode *to_free = list->tail;

  if (!list->tail->prev) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    list->tail = list->tail->prev;
    list->tail->next = NULL;
  }

  list_free_node(to_free);
  list->size--;

  return LIST_SUCCESS;
}

int main(){
  list *l = listCreate(int);
  if (!l)
    printf("fuck1");
  else 
    printf("LISTA C'E");

  if(listInsertStart(l, 5)){
    printf("startOK: %d\n", *(int*)l->head->data);
  }

  if (listInsertEnd(l, 6)){
    printf("endOK: %d\n", *(int*)l->tail->data);
  }

  if (listInsert(l, 7, 1)){
    printf("midOK: %d\n", *(int*)l->tail->data);
  }

  ListNode *current = l->head;
  while (current){
    printf("%d", *(int*)current->data);
    current = current->next;
  }

  listFree(l);
    

}