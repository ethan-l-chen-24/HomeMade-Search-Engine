
/* 
 * word.c - library of word functions
 *
 * see word.h for more information.
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "word.h"
#include <memory.h>

/************** normalizeWord() ******************/
// see word.h for description
void normalizeWord(char* word)
{
    // loop through every character and make it lowercase
    for (char* i = word; *i != '\0'; i++) {
        *i = tolower(*i);
    }
}

/************** intToString() ******************/
// see word.h for description
char* intToString(int x)
{
    // count the number of digits in x
    int xLen = 0;
    int xCopy = x; // create a copy object to not alter the original id
    while (xCopy != 0) {
        xCopy /= 10;
        xLen++;
    }

    // allocate memory to copy the string into
    char* xString = count_malloc(xLen + 1);
    if (xString == NULL) {
        fprintf(stderr, "Error: Out of memory");
        return NULL;
    }
    
    // write the int as a string
    sprintf(xString, "%d", x);
    return xString;
}