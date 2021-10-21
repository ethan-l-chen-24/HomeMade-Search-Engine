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
/* Function saves the contents of a webpage to a file at
 * a given filepath
 *
 * checks if it is possible, and prints the URL, id, and HTML of a 
 * webpage
*/
bool writeToDirectory(char* filepath, webpage_t* page, int* id);

/***************** pageDirValidate() ***********************/
/* Checks if the given directory is a directory created by the file
 *
 * Tries to open a directory's .crawler file. If it exists return true
 * else return false
*/
bool pageDirValidate(char* pageDir);

/***************** loadPageToWebpage() ***********************/
/* Takes a pageDirectory and an ID, builds the filepath of the corresponding
 * crawler file, and loads the webpage and its HTML from this
 *
 * Pseudocode:
 *      1. build the filepath of the crawler file (e.g. ../data/pageDir/1)
 *      2. increment the id
 *      3. try to open the file
 *      4. if the URL is valid, create the webpage, fetch the HTML, and return
*/
webpage_t* loadPageToWebpage(char* pageDir, int* id);

/***************** stringBuilder() ***********************/
/* Function used to build the filepath of a file given
 * a directory and a suffix
 *
 * Note:
 *      if pageDir is null, the filepath will be ../data/end, writing
 *      directly to both directories. If both or null, stringBuilder will
 *      be null
*/
char* stringBuilder(char* pageDir, char* end);


#endif