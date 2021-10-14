/* 
 *  hashtable.c - CS50 'set' module
 * 
 *  see hashtable.h for more information.
 * 
 *  Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"
#include "memory.h"
#include "jhash.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types* ***************/
typedef struct hashtablenode {
    char* key;                  // data for key
    void* item;                 // data for item
    
} hashtablenode_t;

/**************** global types ****************/
typedef struct hashtable {
  int size;                     // the number of bins in the hashtable
  struct set_t** table;         // the hashtable (array of pointers to sets)
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
/* none */

/**************** hashtable_new() ****************/
/* see counters.h for description */
hashtable_t*  hashtable_new(const int num_slots)
{
  if(num_slots > 0) {
    // allocate memory for the hashtable struct and actual table
    hashtable_t* ht = count_malloc(sizeof(hashtable_t));
    ht->table = count_calloc(num_slots, sizeof(set_t*));

    if(ht == NULL || ht->table == NULL) {
      return NULL;         // error allocating structure
    } else {
      // initialize size of the hashtable and table
      ht->size = num_slots;
      return ht;
    }
  } else {
    return NULL;            // invalid index
  }
}

/**************** hashtable_insert() ****************/
/* see counters.h for description */
bool hashtable_insert(hashtable_t* ht, const char* key, void* item)
{	
	if(ht != NULL && key != NULL && item != NULL && hashtable_find(ht, key) == NULL) {
    
    	// get the bin based on the hash function
   		const int hash = JenkinsHash(key, ht->size);
    	set_t* hash_bin =  (set_t *) (ht->table)[hash];

    	// if it doesn't yet exist, create a new bin
    	if(hash_bin == NULL) {
      	(ht->table)[hash] = (void *) set_new();
      	hash_bin = (set_t *) (ht->table)[hash];
    }
    
    return set_insert(hash_bin, key, item);

  } else {
    return false;
  }
}

/**************** hashtable_find() ****************/
/* see counters.h for description */
void* hashtable_find(hashtable_t* ht, const char* key) 
{
    if(ht != NULL && key != NULL) {
      // get the bin based on the hash function
      const int hash = JenkinsHash(key, ht->size);
      set_t* hash_bin = (set_t *) (ht->table)[hash];

      if(hash_bin != NULL) {
        return set_find(hash_bin, key);     // retrieve the item from the set
      } else {
        return false;
      }
    } else {
      return false;
    }
}

/**************** hashtable_print() ****************/
/* see counters.h for description */
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)) 
{
  if(fp != NULL) {
    if(ht != NULL) {
      // traverse each bin in the table
      for(int i = 0; i < ht->size; i++) {
        set_t* hash_bin = (set_t *) (ht->table)[i];
        printf("Bin %d: ", i);
        if(hash_bin != NULL) {
          // print the bin if it exists
          set_print(hash_bin, fp, itemprint);     
        } else {
          fputs("{}\n", fp);
        }
      }
    } else {
      // print null if the set doesn't exist
      fputs("(null)", fp);
    }
  }
}

/**************** hashtable_iterate() ****************/
/* see counters.h for description */
void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) )
{
    if(ht != NULL && itemfunc != NULL) {
      // go through each bin in the table
      for(int i = 0; i < ht->size; i++) {
        set_t* hash_bin = (set_t *) (ht->table)[i];
        if(hash_bin != NULL) { 
          // iterate over the sets, passing the iterate function
          set_iterate(hash_bin, arg, itemfunc);
        }
      }
    }
}

/**************** hashtable_delete() ****************/
/* see counters.h for description */
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) )
{
  if(ht != NULL) {
    for(int i = 0; i < ht->size; i++) {
      if(itemdelete != NULL) {              // if possible
        set_t* hash_bin = (set_t *) (ht->table)[i];   
        set_delete(hash_bin, itemdelete);   // delete the set/bin
      }
    }
    count_free(ht->table);                    // free the table
    count_free(ht);                           // and the struct
  }

#ifdef MEMTEST
  count_report(stdout, "End of hashtable_delete");
#endif  
}