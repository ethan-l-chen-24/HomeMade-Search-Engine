/* 
 * pagedir.c - library of directory functions
 *
 * see pagedir.h for more information.
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "../libcs50/memory.h"

/************** validDirectory() ******************/
// see pagedir.h for description
bool validDirectory(char* directoryName) 
{
    // count the number of characters that will be in the filepath string
    char begin[] = "../common";
    char end[] = ".crawler";
    int newSize = sizeof(directoryName) + sizeof(begin) + sizeof(end) + 1;

    // allocate the space and build that filepath
    char* testFile = count_malloc(newSize);
    sprintf(testFile, "%s/%s/%s", begin, directoryName, end);

    // check if that directory exists by attempting to write to that file
    FILE *fp = fopen(testFile, "w");
    if(testFile != NULL) free(testFile);
    if(fp != NULL) {
        // success, write arbitrary lines in that file
        fprintf(fp, "This file is created by 'pagedir' to check if this directory exists");
        fclose(fp);
        return true;
    } else {
        // if it fails, print error and return false for use in crawler.c
        fprintf(stderr, "Error: directory %s is invalid!", directoryName);
        return false;
    }
}