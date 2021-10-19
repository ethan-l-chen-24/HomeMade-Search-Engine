/* 
 * index.h - header file for CS50 'index' file in 'common' module
 *
 * p
 * 
 * Ethan Chen, October 2021
 */

#ifndef __PAGE_DIR
#define __PAGE_DIR

#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"

/******************* functions *******************/

/******************* validDirectory() ********************/
/* Function used to check if a directory exists
 * 
*/
bool saveIndex(char* directoryName);

/******************* validDirectory() ********************/
/* Function used to check if a directory exists
 * 
*/
hashtable_t* loadIndex(char* directoryName);

bool indexFile(FILE* fp, hashtable_t* index, int id);

#endif