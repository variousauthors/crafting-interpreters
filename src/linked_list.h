#include "clox_memory.h"
#include <cstddef>
#include <cstdint>

struct Node {
  char *value;
  int next;
};

// node list, with append and dynamic grow

struct NodeList {
  int count;
  int capacity;
  Node *nodes;
};

void initNodeList(NodeList *list) {
  list->capacity = 0;
  list->count = 0;
  list->nodes = NULL;
}

void freeNodeList(NodeList *list) {
  FREE_ARRAY(NodeList, list, list->count);
  list->count = 0;
  list->capacity = 0;
}

void insertNodeList(NodeList *list, Node newNode) {
  if (list->count + 1 > list->capacity) {
    int oldCapacity = list->capacity;
    list->capacity = GROW_CAPACITY(oldCapacity);
    list->nodes = GROW_ARRAY(Node, list->nodes, oldCapacity, list->capacity);
  }

  list->nodes[list->count] = newNode;
  list->count++;
}

// []

#define END_OF_LIST -1

void append(NodeList *list, Node newNode) {
  int i = 0;
  Node *prev = &list->nodes[i];

  if (prev == NULL) {
    prev->next = i + 1;
    newNode.next = END_OF_LIST;
    return insertNodeList(list, newNode);
  }

  // advance to the end of the list
  while (prev->next != END_OF_LIST) {
    i = prev->next;
    prev = &list->nodes[prev->next];
  }

  // prev is the last node
  // so set current to the next index

  Node *current = &list->nodes[i];

  // otherwise find an empty spot
  while (current->value != NULL) {
    current = &list->nodes[i];

    if (current == NULL) {
      prev->next = i;
      newNode.next = END_OF_LIST;
      return insertNodeList(list, newNode);
    } 
    
    i++;
  }

  // now current is pointing to a tombstone
  prev->next = i;
  current->value = newNode.value;
  current->next = END_OF_LIST;

  return;
}
