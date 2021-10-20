/* 
 * pagedir.h - header file for CS50 'pagedir' file in 'common' module
 *
 * provides a library for page directory functions
 * 
 * Ethan Chen, October 2021
 */

#ifndef __PAGE_DIR
#define __PAGE_DIR

#include <stdbool.h>
#include "../libcs50/webpage.h"

/******************* functions *******************/

/******************* validDirectory() ********************/
/* Function used to check if a directory exists
 * 
 * Simultaneously writes an arbitrary file called '.crawler' to that directory
 * if it does exist, otherwise prints an error and returns false
*/
bool validDirectory(char* directoryName);

/***************** writeToDirectory() ***********************/
/* Function used to save the contents of a webpage to a file at
 * a given filepath
 *
 * checks if it is possible, and prints the URL, id, and HTML of a 
 * webpage
*/
bool writeToDirectory(char* filepath, webpage_t* page, int* id);

/***************** stringBuilder() ***********************/
/* Function used to build the filepath of a file given
 * a directory and a suffix
 *
 * returns the path
*/
char* stringBuilder(char* pageDir, char* end);


#endif