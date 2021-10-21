
/* 
 * index.c - library to read and write index files
 *
 * see index.h for more information.
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdlib.h>
#include <stdbool.h>
#include "index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/memory.h"
#include "pagedir.h"
#include "word.h"

/************* global types ****************/

typedef struct index {
    hashtable_t* table;
} index_t;

/************* local function prototypes ********************/

static void printCT(void* arg, const char* key, void* item);
static void printCTHelper(void* arg, const int key, const int count);
static bool readWordsInFile(webpage_t* page, index_t* index, int id);
static void deleteCT(void* item);

/************** newIndex() ******************/
// see index.h for description
index_t* newIndex(const int tableSize)
{
    // allocate memory for the index
    index_t* index = count_malloc(sizeof(index_t));
    if(index != NULL) { 
        // set the inner hashtable to a new hashtable of the specified size
        if((index->table = hashtable_new(tableSize)) != NULL) return index;
        else return NULL;
    } else {
        fprintf(stderr, "Error: out of memory");
        return NULL;
    }
}

/************** newIndex() ******************/
// see index.h for description
void deleteIndex(index_t* index) 
{
    if(index != NULL) {
        if(index->table != NULL) {
            // free the hashtable
            hashtable_delete(index->table, deleteCT);
        }
        // free the struct
        free(index);
    }
} 

/************** saveIndex() ******************/
// see index.h for description
bool saveIndexToFile(char* filename, index_t* index)
{
    // build the filepath of the index file
    char* filepath = stringBuilder(NULL, filename);
    printf("%s", filepath);
    FILE* fp;
    // try to open that file (should work as long as dir exists)
	if((fp = fopen(filepath, "w")) != NULL) {
        // iterate through the table and print the file in the format specified
		hashtable_iterate(index->table, fp, printCT);
        fclose(fp);
	}
    free(filepath);
    return true;
}

/************** loadIndex() ******************/ // COMMENT THIS 
// see index.h for description
index_t* loadIndex(char* filepath)
{
    index_t* index = newIndex(800);
    if(index == NULL) {
        fprintf(stderr, "Error: Out of memory");
        return NULL;
    }
    char* indexFilePath = stringBuilder(NULL, filepath);
    if(indexFilePath == NULL) {
        fprintf(stderr, "filepath could not be built");
        return NULL;
    }
    FILE* fp = fopen(indexFilePath, "r");
    if(fp != NULL) {
        char* word;
        while((word = freadwordp(fp)) != NULL) {
            counters_t* wordFreqCtr = counters_new();
            if(wordFreqCtr == NULL) return NULL;
            if(!hashtable_insert(index->table, word, wordFreqCtr)) {
                fprintf(stderr, "Error: could not insert into index");
                return NULL;
            }
    
            int id;
            int count;
            while(fscanf(fp, "%d %d ", &id, &count) == 2) {
                counters_set(wordFreqCtr, id, count);
            }
        }
    } else {
        fprintf(stderr, "Error: invalid filepath");
        return NULL;
    }
}

/************** indexWebpage() ******************/
// see index.h for description
bool indexWebpage(index_t* index, webpage_t* webpage, int id) 
{
    char* URL = webpage_getURL(webpage);
    // read the words in the file and insert them into the index
    if(!readWordsInFile(webpage, index, id)) {
        fprintf(stderr, "Error: page %s cannot be read\n", URL);
        return false;
    }
    // delete the webpage and its inner hashtable
    webpage_delete(webpage);
    return true;
}

/************** readWordsInFile() ******************/
/*
 * increments through every word in the file and inserts it into the index
 *
 * Pseudocode:
 *      1. loop over all of the words
 *      2. make the word lowercase
 *      3. check if the word already exists in the index
 *      4. if it doesn't create an entry with a counterset as the item
 *      5. if it does, load that counterset
 *      6. insert the id into that counterset
*/
static bool readWordsInFile(webpage_t* page, index_t* index, int id)
{
    int loc = 0;
    char* word;
    // read through every WORD in the webpage
    while ((word = webpage_getNextWord(page, &loc)) != NULL) {
        // make the word lowercase
        normalizeWord(word);
        // check if the item exists in the index
        void* item = hashtable_find(index->table, word);
        if (item == NULL) {
            // if it doesn't create an entry for the word in the index and insert the id
            counters_t* wordCounter = counters_new();
            counters_add(wordCounter, id);
            hashtable_insert(index->table, word, wordCounter);
        } else {
            // if it already exists, add the id to the counters
            counters_t* wordCounter = (counters_t*) item;
            counters_add(wordCounter, id);
        }
        free(word);
    }
    return true;
}

/************** printCT() ******************/
/*
 * prints out the index in the correct formatting
 *
 * Formatting:
 *      should print every word, followed by a sequence of pairs of id numbers and counts
 *      separated by spaces
 *  
 * Examples:
 *      word [id] [count] [id] [count] [id] [count]
 *      hello 2 3 3 7 4 1
 *      world 3 4 4 3 7 5 5 9
*/
static void printCT(void* arg, const char* key, void* item) 
{
    if(arg != NULL) {
        // cast the arg to a file
	    FILE* fp = (FILE*) arg;
        if(item != NULL) {
            // print the key (word)
	        fprintf(fp, "%s ", key);
            // iterate through the counters and print the key and item
	        counters_t* ctrs = (counters_t*) item;
	        counters_iterate(ctrs, arg, printCTHelper);
        }
        // line break
	    fprintf(fp, "\n");
    }
}

/************** printCTHelper() ******************/
/* a helper that helps print out the counterset */
static void printCTHelper(void* arg, const int key, const int count) 
{
    if(arg != NULL) {
        // print the key and count to the file
        FILE* fp = (FILE*) arg;
	    fprintf(fp, "%d %d ", key, count);
    }
}

/************* deleteCT() *************/
/* a helper function to help the hashtable delete its counter objects */
static void deleteCT(void* item)
{
    // cast to a counters struct so we can delete it
	counters_t* ct = item;
	if (ct != NULL) {
		counters_delete(ct);
	}
}