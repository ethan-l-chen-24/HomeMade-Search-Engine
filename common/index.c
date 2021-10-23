
/* 
 * index.c - library to read and write index files
 *
 * see index.h for more information.
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "word.h"
#include "hashtable.h"
#include "counters.h"
#include "webpage.h"
#include "file.h"
#include "memory.h"

/************* global types ****************/

typedef struct index {
    hashtable_t* table;
} index_t;

/************* local function prototypes ********************/

static void loadWordInIndex(index_t* index, char* word, FILE* fp);
static void printCT(void* arg, const char* key, void* item);
static void printCTHelper(void* arg, const int key, const int count);
static void readWordsInWebpage(webpage_t* page, index_t* index, int* id);
static void deleteCT(void* item);

/************** newIndex() ******************/
// see index.h for description
index_t* newIndex(const int tableSize)
{
    // allocate memory for the index
    index_t* index = count_malloc(sizeof(index_t));
    if (index != NULL) { 
        // set the inner hashtable to a new hashtable of the specified size
        if ((index->table = hashtable_new(tableSize)) != NULL) return index;
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
    if (index != NULL) {
        if (index->table != NULL) {
            // free the hashtable
            hashtable_delete(index->table, deleteCT);
        }
        // free the struct
        count_free(index);
    }
} 

/************** buildIndexFromCrawler() ******************/
// see index.h for description
bool buildIndexFromCrawler(char* pageDir, index_t* index) 
{
    // validate parameters
    if (pageDir != NULL && index != NULL) {

        // loop through as long as the file exists 
        int id = 1; 
        // load the crawler files into webpages as long as they exist
        webpage_t* crawlerPage;  
        while ((crawlerPage = loadPageToWebpage(pageDir, id)) != NULL) {
            // index them
            if (!indexWebpage(index, crawlerPage, &id)) {
                fprintf(stderr, "Error: couldn't index page");
            }
        }
        return true;
    } else {
        fprintf(stderr, "Error: Null-Pointer Exception");
        return false;
    }
}

/************** saveIndexToFile() ******************/
// see index.h for description
bool saveIndexToFile(char* filename, index_t* index)
{
    // build the filepath of the index file
    if (filename == NULL || index == NULL) return false;
    char* filepath = stringBuilder(NULL, filename);
    if (filepath == NULL) return false;

    FILE* fp;
    // try to open that file (should work as long as dir exists)
	if ((fp = fopen(filepath, "w")) != NULL) {
        // iterate through the table and print the file in the format specified
		hashtable_iterate(index->table, fp, printCT);
        fclose(fp);
	}
    count_free(filepath);
    return true;
}

/************** loadIndex() ******************/ // COMMENT THIS 
// see index.h for description
index_t* loadIndexFromFile(char* filepath)
{
    if (filepath == NULL) return NULL;

    // create the index
    index_t* index = newIndex(800);
    if (index == NULL) {
        fprintf(stderr, "Error: Out of memory");
        return NULL;
    }

    // build the filepath
    char* indexFilePath = stringBuilder(NULL, filepath);
    if (indexFilePath == NULL) {
        fprintf(stderr, "filepath could not be built\n");
        return NULL;
    }

    // open the index file
    printf("Reading file %s\n", indexFilePath);
    FILE* fp = fopen(indexFilePath, "r");
    count_free(indexFilePath);
    if (fp != NULL) {
        // read the first word in the line as long as it exists
        // and put that word into the index
        char* word;
        while ((word = freadwordp(fp)) != NULL) loadWordInIndex(index, word, fp); 
        fclose(fp);
        return index;
    } else {
        // handle errors, free memory
        deleteIndex(index);
        fprintf(stderr, "Error: invalid filepath");
        return NULL;
    }
}

/************** getHashtable() ******************/
// see index.h for description
hashtable_t* getHashtable(index_t* index)
{
    if(index != NULL) {
        return index->table;
    } else {
        return NULL;
    }
}

/************** indexWebpage() ******************/
// see index.h for description
bool indexWebpage(index_t* index, webpage_t* webpage, int* id) 
{
    if (index == NULL || webpage == NULL || *id < 0) return false;
    // read the words in the file and insert them into the index
    readWordsInWebpage(webpage, index, id);
    // delete the webpage and its inner hashtable
    webpage_delete(webpage);
    return true;
}

/************** loadWordInIndex() ******************/
/*
 * adds a word to the index from the index file
 *
 * Pseudocode:
 *      1. creates a counterset
 *      2. inserts the counterset into hashtable as a value with 
 *          the word as the key
 *      3. scan for pairs of ints and add them to the counterset
*/
static void loadWordInIndex(index_t* index, char* word, FILE* fp) 
{
    if (index == NULL || word == NULL || fp == NULL) return;
    // create a new counter to insert into the index
    counters_t* wordFreqCtr = counters_new();
    if (wordFreqCtr == NULL) return;
    // insert the word into the hashtable
    if (!hashtable_insert(index->table, word, wordFreqCtr)) {
        fprintf(stderr, "Error: duplicate words");
    }
    count_free(word);
    
    // scan the rest of the line for pairs of ints, stop when no longer found
    int id;
    int count;
    while (fscanf(fp, "%d %d ", &id, &count) == 2) {
        // add the pairs to the index where the first is the id and the 
        // second is the count
        counters_set(wordFreqCtr, id, count);
    }
}

/************** readWordsInWebpage() ******************/
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
static void readWordsInWebpage(webpage_t* page, index_t* index, int* id)
{
    if (page == NULL || index == NULL || *id < 0) return;

    int loc = 0;
    char* word;
    // read through every WORD in the webpage
    while ((word = webpage_getNextWord(page, &loc)) != NULL) {
        // make the word lowercase and check length
        normalizeWord(word);
        if (strlen(word) < 3) {
            count_free(word);
            continue;
        }
        // check if the item exists in the index
        void* item = hashtable_find(index->table, word);
        if (item == NULL) {
            // if it doesn't create an entry for the word in the index and insert the id
            counters_t* wordCounter = counters_new();
            counters_add(wordCounter, *id);
            hashtable_insert(index->table, word, wordCounter);
        } else {
            // if it already exists, add the id to the counters
            counters_t* wordCounter = (counters_t*) item;
            counters_add(wordCounter, *id);
        }
        count_free(word);
    }
    // increment id
    (*id)++;
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
    if (arg == NULL || key == NULL || item == NULL) return;
    // cast the arg to a file
    FILE* fp = (FILE*) arg;
    if (item != NULL) {
        // print the key (word)
        fprintf(fp, "%s ", key);
        // iterate through the counters and print the key and item
	    counters_t* ctrs = (counters_t*) item;
        counters_iterate(ctrs, arg, printCTHelper);
    }
    // line break
	fprintf(fp, "\n");
}

/************** printCTHelper() ******************/
/* a helper that helps print out the counterset */
static void printCTHelper(void* arg, const int key, const int count) 
{
    if (arg != NULL) {
        // print the key and count to the file
        FILE* fp = (FILE*) arg;
	    fprintf(fp, "%d %d ", key, count);
    }
}

/************* deleteCT() *************/
/* a helper function to help the hashtable delete its counter objects */
static void deleteCT(void* item)
{
    if (item == NULL) return;
    // cast to a counters struct so we can delete it
	counters_t* ct = item;
	if (ct != NULL) {
		counters_delete(ct);
	}
}

