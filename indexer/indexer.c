/* 
 * indexer.c - crawler module for tiny search engine 
 *
 * given a directory previously accessed and filled by the crawler, the indexer reads
 * each of the files, rebuilds the webpage, and extracts every word from the HTML to build
 * an "index". This index keeps track of the word, the file IDs that contain it, and the 
 * number of occurrences in that file. It will also create and print an output file
 * to the same directory with each word followed by pairs of [fileID] [numberOccurrences]
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/counters.h"
#include "../libcs50/memory.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"

/************* function prototypes ********************/

bool indexer(char* pageDir, char* indexFilename);
bool buildIndex(char* pageDir, index_t* index);

/************** main() ******************/
/* the "testing" function/main function, which takes two arguments 
 * as inputs (other than the executable call), the directory containing the
 * files to index and the name of the index file to write
 * 
 * Pseudocode:
 *      1. Make sure there are exactly 2 arguments
 *      2. copy the pageDirectory and indexFilename into malloc'd strings
 *      3. call the indexer method
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
 *      2. the directory exists, otherwise throws errors
*/
int main(const int argc, char* argv[])
{
    char* program = argv[0];
    // check for the appropriate number of arguments
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [pageDirectory] [indexFilename]\n", program);
        return 1;
    }

    // allocate memory and copy string for pageDir
    char* pageDirArg = argv[1];
    char* pageDir = count_malloc(strlen(pageDirArg) + 1);
    if (pageDir == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(pageDir, pageDirArg);

    // allocate memory and copy string for indexFilename
    char* indexFnameArg = argv[2];
    char* indexFilename = count_malloc(strlen(indexFnameArg) + 1);
    if (indexFilename == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(indexFilename, indexFnameArg);

    // check if the directory exists
    if(!validDirectory(pageDir)) {
        return false;
    }

    // run the indexer
    if(indexer(pageDir, indexFilename)) return 0;
    else return 1;
}

/************** indexer() ******************/
/* The skeleton code for the indexer, which creates the index
 * and appropriately deals with memory
 * 
 * Pseudocode:
 *      1. create the index
 *      2. call buildIndex and saveIndex
 *      3. appropriately free memory
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
*/
bool indexer(char* pageDir, char* indexFilename) 
{
    // check validity of arguments
    if(pageDir != NULL && indexFilename != NULL) {
        // initialize the index
        index_t* index = newIndex(800);
        // build the index from the crawler files
        if(!buildIndex(pageDir, index)) {
            return false;
        }
        // save the index to the given filename
        if(!saveIndex(indexFilename, pageDir, index)) {
            return false;
        }
        // free up memory
        deleteIndex(index);
        count_free(pageDir);
        count_free(indexFilename);
        return true;
    } else {
        fprintf(stderr, "Error: Null-Pointer Exception");
        return false;
    }
}

/************** buildIndex() ******************/
/* the "testing" function/main function, which takes two arguments 
 * as inputs (other than the executable call), the directory containing the
 * files to index and the name of the index file to write
 * 
 * Pseudocode:
 *      1. Given the directory, build the filepath directory/1
 *      2. open the file if possible and index the data
 *      3. increment to the next file, filepath directory/2...
 *      4. increment until the file doesn't exist
 * 
 * Assumptions:
 *      1. All of the files inside of the directory are labeled 1-numFiles
*/
bool buildIndex(char* pageDir, index_t* index) 
{
    // validate parameters
    if(pageDir != NULL && index != NULL) {
        // begin at file id 1
        int id = 1;

        // build the filepath
        char* idString = intToString(id);
        char* file = stringBuilder(pageDir, idString);

        // loop through as long as the file exists
        FILE* fp;
        while ((fp = fopen(file, "r")) != NULL) {
            printf("Reading file %s\n", file);
            // index the file's data
            indexCrawlerFile(fp, index, id);
            // free up memory
            fclose(fp);
            count_free(file);
            count_free(idString);

            // increment to the next case (next ID)
            id++;
            idString = intToString(id);
            file = stringBuilder(pageDir, idString);
        }
        // free up memory
        count_free(file);
        count_free(idString);
        return true;
    } else {
        fprintf(stderr, "Error: Null-Pointer Exception");
        return false;
    }
}