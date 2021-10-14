/* 
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
  char* key;            // pointer to data for the key
  void* item;           // pointer to data for the item
  struct setnode* next; // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode* head; // head of the list of items in set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t* setnode_new(char* key, void* item);

/**************** set_new() ****************/
/* see set.h for description */
set_t* set_new(void)
{
  // allocate memory for the set
  set_t* set = count_malloc(sizeof(set_t));

  if (set == NULL)
  {
    return NULL; // error allocating set
  }
  else
  {
    // initialize contents of the set structure
    set->head = NULL;
    return set;
  }
}

/**************** set_insert() ****************/
/* see set.h for description */
bool set_insert(set_t* set, const char* key, void* item)
{
  if (set != NULL && item != NULL && key != NULL) {

    // copy over the values of key and item to new pointers
    int itemSize = sizeof(item);
    char* key_copy = count_malloc(sizeof(key));
    void* item_copy = count_malloc(itemSize);
    strcpy(key_copy, key);
    memcpy(item_copy, item, itemSize);

    // check if the key already exists in the set
    if (set_find(set, key_copy) == NULL) {
      setnode_t* new = setnode_new(key_copy, item_copy);

      if (new != NULL) {
        // add it to the head of the list
        new->next = set->head;
        set->head = new;
      }
      return true;
    } else {
      // free the copies if the key already exists
      free(key_copy);
      free(item_copy);
      return false;
    }
  } else {
    return false;
  }

#ifdef MEMTEST
  count_report(stdout, "After set_insert");
#endif
}

/**************** setnode_new ****************/
/* allocate and initialize a setnode */
static setnode_t* setnode_new(char* key, void* item)
{
  // allocate memory for the node
  setnode_t* node = count_malloc(sizeof(setnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->item = item;
    node->key = key;
    node->next = NULL;
    return node;
  }
}

/**************** set_find() ****************/
/* see set.h for description */
void* set_find(set_t* set, const char* key)
{
  if (set != NULL && key != NULL) {
    // traverse each node in the list
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
      // check if the key parameter matches equates the node of the key
      if (strcmp(key, node->key) == 0) {
        if (node->item != NULL) {
          return node->item;
        } else {
          return NULL;
        }
      }
    }
    return NULL;
  } else {
    return NULL;
  }
}

/**************** set_print() ****************/
/* see set.h for description */
void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item))
{
  if (fp != NULL) {
    if (set != NULL) {
      fputc('{', fp);
      // traverse each node
      for (setnode_t* node = set->head; node != NULL; node = node->next) {
        if (itemprint != NULL) {
          // print the nsode's key and item
          (*itemprint)(fp, node->key, node->item);
          fputc(',', fp);
        }
      }
      fputs("}\n", fp);
    } else {
      // print null if the set doesn't exist
      fputs("(null)\n", fp);
    }
  }
}

/**************** set_iterate() ****************/
/* see set.h for description */
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item))
{
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item);
    }
  }
}

/**************** set_delete() ****************/
/* see set.h for description */
void set_delete(set_t* set, void (*itemdelete)(void* item))
{
  if (set != NULL) {
    for (setnode_t* node = set->head; node != NULL;) {
      if (itemdelete != NULL) {                            // if possible...
        (*itemdelete)(node->item); // delete node's item
        (*itemdelete)(node->key);  // delete node's key
      }
      setnode_t* next = node->next; // remember what comes next
      count_free(node);               // free the node
      node = next;                  // and move on to next
    }
    count_free(set);
  }

#ifdef MEMTEST
  count_report(stdout, "End of set_delete");
#endif
}