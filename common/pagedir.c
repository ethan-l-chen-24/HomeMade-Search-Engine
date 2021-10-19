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
#include "../libcs50/webpage.h"

/************** validDirectory() ******************/
// see pagedir.h for description
bool validDirectory(char* directoryName) 
{
    // count the number of characters that will be in the filepath string
    char begin[] = "../data";
    char end[] = ".crawler";
    int counter = 0;
    int newSize = strlen(directoryName) + strlen(begin) + strlen(end) + 3;

    // allocate the space and build that filepath
    char* testFile = count_malloc(newSize);
    sprintf(testFile, "%s/%s/%s", begin, directoryName, end);

    // check if that directory exists by attempting to write to that file
    FILE *fp = fopen(testFile, "w");
    if (testFile != NULL) free(testFile);
    if (fp != NULL) {
        // success, write arbitrary lines in that file
        fprintf(fp, "This file is created by 'pagedir' to check if this directory exists");
        fclose(fp);
        return true;
    } else {
        // if it fails, print error and return false for use in crawler.c
        fprintf(stderr, "Error: directory %s is invalid!\n", directoryName);
        return false;
    }
}

/************** writeToDirectory() ******************/
// see pagedir.h for description
bool writeToDirectory(char* filepath, webpage_t* page, int* id) 
{
    FILE *fp = fopen(filepath, "w");
    if (fp != NULL) {
        // get the URL, depth, and HTML from the webpage
        char* pageURL = webpage_getURL(page);
        int pageDepth = webpage_getDepth(page);
        char* pageHTML = webpage_getHTML(page);

        // print the URL, depth, and HTML to the file
        if (pageURL != NULL) fprintf(fp, "%s\n", pageURL);
        fprintf(fp, "%d\n", pageDepth);
	    if (pageHTML != NULL) fprintf(fp, "%s", pageHTML);
            
        fclose(fp); // close the file

        *id += 1; // increment the id
        return true;
    } else {
        // handle errors
        fprintf(stderr, "Error: could not create file %s\n", filepath);
        return false;
    }
    
}

/************** stringBuilder() ******************/
// see pagedir.h for description
char* stringBuilder(char* pageDir, char* end) 
{
    if (pageDir != NULL) {
        
        // count the number of characters that will be in the final filepath string
        char prefix[] = "../data/";
        char slash[] = "/";
        int destSize = strlen(end) + strlen(pageDir) + strlen(prefix) + strlen(slash) + 1; // +1 for /0 character

        // allocate the space and print the filepath to that memory
        char* filename = count_malloc(destSize);
        sprintf(filename, "../data/%s/%s", pageDir, end);
        return filename;
    } else {
        return NULL;
    }
}

/************** stringBuilder() ******************/
// see pagedir.h for description
char* intToString(int x)
{
    int xLen = 0;
    int xCopy = x; // create a copy object to not alter the original id
    while (xCopy != 0) {
        xCopy /= 10;
        xLen++;
    }
    char* xString = count_malloc(xLen + 1);
    if(xString == NULL) {
        fprintf(stderr, "Error: Out of memory");
        return NULL;
    }
    sprintf(xString, "%d", x);
    return xString;
}