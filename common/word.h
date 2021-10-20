/* 
 * word.h - header file for CS50 'word' file in 'common' module
 *
 * provides a library of word-related functions
 * 
 * Ethan Chen, October 2021
 */

#ifndef __WORD
#define __WORD

#include <stdbool.h>

/******************* functions *******************/

/***************** normalizeWord() ***********************/
/* makes a word all lowercase, if the character can be made lowercase
*/
void normalizeWord(char* word);

/***************** intToString() ***********************/
/* turns an int into a string
 *
 * returns the path
*/
char* intToString(int x);


#endif