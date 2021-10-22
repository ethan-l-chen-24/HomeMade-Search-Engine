/* 
 * indextest.c - crawler module for tiny search engine 
 *
 * takes the name of an output file from the indexer as well as a file to write output to.
 * The indexTest will test the loadFile() function from index.h by loading the entire
 * hashtable from the index file and then writing it back out to the output file. 
 * By comparing the output to the input to make sure they are the same, this process
 * validates the loadFile() function
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "memory.h"

/************* function prototypes ********************/

bool indexTest(char* oldFile, char* newFile);

/************** main() ******************/
/* the "testing" function/main function, which takes two arguments 
 * as inputs (other than the executable call), the filename of an index file
 * and the filename of an output file to be created
 * 
 * Pseudocode:
 *      1. Make sure there are exactly 3 arguments
 *      2. copy the pageDirectory and indexFilename into malloc'd strings
 *      3. call the indexer method
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
 *      2. the index file is located within the data directory
*/
int main(const int argc, char* argv[]) 
{
    // validate arguments
    char* program = argv[0];
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [oldIndexFilename] [newIndexFilename]", program);
        return 1;
    }
  
    // allocate memory and copy string for pageDir
    char* oldFileArg = argv[1];
    char* oldFile = count_malloc(strlen(oldFileArg) + 1);
    if (oldFile == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(oldFile, oldFileArg);

    // allocate memory and copy string for indexFilename
    char* newFileArg = argv[2];
    char* newFile = count_malloc(strlen(newFileArg) + 1);
    if (newFile == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(newFile, newFileArg);

    // run the test
    if(indexTest(oldFile, newFile)) return 1;
    else return 0;
}

/************** indexTest() ******************/
/* runs the test by loading the index file into an index
 * and then saving it back to a file
 * 
 * Pseudocode:
 *      1. Make sure there are exactly 3 arguments
 *      2. copy the pageDirectory and indexFilename into malloc'd strings
 *      3. call the indexer method
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
 *      2. the index file is located within the data directory
*/
bool indexTest(char* oldFile, char* newFile) 
{
    // load the index from the file
    index_t* index = loadIndex(oldFile);
    printf("Index loaded\n");
    if (index != NULL) {
        // save the index to the new file
        if (saveIndexToFile(newFile, index)) {
            printf("Index saved\n");
            deleteIndex(index);
            count_free(newFile);
            count_free(oldFile);
            return 0;
        } else { // handle errors
            deleteIndex(index);
            count_free(newFile);
            count_free(oldFile);    
            printf("Something didn't work\n");
            return 1;
        }
    } else { // handle errors
        count_free(newFile);
        count_free(oldFile);
        printf("test failed");
        return 1;
    }
}