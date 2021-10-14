/* 
 * counters.c - CS50 'set' module
 *
 * see counters.h for more information.
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct countersnode {
  int key;                   // data for key
  int count;                 // count corresponding to the key
  struct countersnode* next; // link to next node
} countersnode_t;

/**************** global types ****************/
typedef struct counters {
  struct countersnode* head; // head of the list of items in set
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static countersnode_t* countersnode_new(int key);
static countersnode_t* getcountersnode(counters_t *ctrs, const int key);

/**************** counters_new() ****************/
/* see counters.h for description */
counters_t* counters_new(void)
{
  // allocate memory for the counterset
  counters_t* counters = count_malloc(sizeof(counters_t));

  if (counters == NULL) {
    return NULL; // error allocating counterset
  } else {
    // initialize contents of the counterset structure
    counters->head = NULL;
    return counters;
  }
}

/**************** counters_add() ****************/
/* see counters.h for description */
int counters_add(counters_t *ctrs, const int key)
{
  if (ctrs != NULL && key >= 0) {
    // check if the key has been added before
    if (counters_get(ctrs, key) == 0) {
      countersnode_t *new = countersnode_new(key);

      if (new != NULL) {
        // add it to the head of the list
        new->next = ctrs->head;
        ctrs->head = new;
        return new->count;
      } else {
        // error, out of memory
        return 0;
      }
    } else {
      // increment the counter for the given key
      countersnode_t* node = getcountersnode(ctrs, key);
      node->count++;
      return node->count;
    }
  } else {
    // invalid key or null counterset, return 0
    return 0;
  }

#ifdef MEMTEST
  count_report(stdout, "After counters_add");
#endif
}

/**************** countersnode_new() ****************/
/* allocate and initialize a countersnode */
static countersnode_t* countersnode_new(int key)
{
  // allocate memory for the node
  countersnode_t* node = count_malloc(sizeof(countersnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->count = 1;
    node->key = key;
    node->next = NULL;
    return node;
  }
}

/**************** getcountersnode() ****************/
/* return the countersnode at the given key */
static countersnode_t* getcountersnode(counters_t *ctrs, const int key)
{
  if (ctrs != NULL && key >= 0) {
    // traverse each node in the list
    for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
      // return the node with the corresponding key
      if (key == node->key) {
        return node;
      }
    }
    // return error if the node does not exist
    return NULL;
  } else {
    return NULL;
  }
}

/**************** counters_get() ****************/
/* see counters.h for description */
int counters_get(counters_t *ctrs, const int key)
{
  if (ctrs != NULL && key >= 0) {
    // get the node that contains the key
    countersnode_t* node = getcountersnode(ctrs, key);
    if (node != NULL) {
      // return that node's count, if it exists
      return node->count;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

/**************** counters_set() ****************/
/* see counters.h for description */
bool counters_set(counters_t* ctrs, const int key, const int count)
{
  if (ctrs != NULL && key >= 0 && count >= 0) {
    // check if the node exists or not
    if (counters_get(ctrs, key) == 0) {
      counters_add(ctrs, key);
    }

    // set the count of the node to the argument, return true
    countersnode_t* node = getcountersnode(ctrs, key);
    node->count = count;
    return true;
  } else {
    return false;
  }
}

/**************** counters_print() ****************/
/* see counters.h for description */
void counters_print(counters_t *ctrs, FILE *fp)
{
  if (fp != NULL) {
    if (ctrs != NULL) {
      fputc('{', fp);
      // traverse each node
      for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
        // print the node's key and counter in form key=counter
        fprintf(fp, "%d=%d, ", node->key, node->count);
      }
      fputc('}', fp);
    } else {
      // print null if the counterset doesn't exist
      fputs("(null)", fp);
    }
  }
}

/**************** counters_iterate() ****************/
/* see counters.h for description */
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count))
{
  if (ctrs != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->count);
    }
  }
}

/**************** counters_delete() ****************/
/* see counters.h for description */
void counters_delete(counters_t *ctrs)
{
  if (ctrs != NULL) {
    for (countersnode_t* node = ctrs->head; node != NULL;) {
      countersnode_t* next = node->next; // remember what comes next
      count_free(node);                    // free the node
      node = next;                       // move on to the next
    }
    count_free(ctrs);
  }

#ifdef MEMTEST
  count_report(stdout, "End of counters_delete");
#endif
}