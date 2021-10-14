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

/******************* functions *******************/

/******************* validDirectory ********************/
/* Function used to check if a directory exists
 * 
 * Simultaneously writes an arbitrary file called '.crawler' to that directory
 * if it does exist, otherwise prints an error and returns false
*/
bool validDirectory(char* directoryName);

#endif