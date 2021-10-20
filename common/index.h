/* 
 * index.h - header file for CS50 'index' file in 'common' module
 *
 * provides a set of functions that can index a file, save it to a file, or load 
 * from an index file into a hashtable
 * 
 * Ethan Chen, October 2021
 */

#ifndef __INDEX
#define __INDEX

#include <stdbool.h>
#include <stdio.h>

/**************** global types ****************/
typedef struct index index_t; // holds the hashtable used for indexing

/******************* functions *******************/

/******************* newIndex() ******************/
/*
 * Function used to create a new index struct
 * It creates a hashtable with the given size
*/
index_t* newIndex(const int tableSize);

/******************* deleteIndex() ******************/
/* deletes an index struct */
void deleteIndex(index_t* index);

/******************* saveIndex() ********************/
/* Function used to save an index hashtable to a file in a given directory
 *
 *  Pseudocode:
 *      1. open the file to write to it
 *      2. Iterate through the items in the hashtable
 *      3. Print the word followed by the counters key and item using CTprint
*/
bool saveIndex(char* filename, char* pageDir, index_t* index);

/******************* loadIndex() ********************/
/* Function used to read an index file and load the hashtable
 *
 * Pseudocode:
 *      1. 
 *      2. 
*/
index_t* loadIndex(char* directoryName);

/******************* indexCrawlerFile() ********************/
/* Takes a file in the crawler format and loads its words into the hashtable
 *
 * Pseudocode:
 *      1.  open the file and read the first line
 *      2. rebuild the webpage struct, validate, and fetch the HTML
 *      3. read the file's words and insert them into the hashmap
 *      4. delete the webpage
*/
bool indexCrawlerFile(FILE* fp, index_t* index, int id);

#endif