/* 
 * querier.c - querier module for tiny search engine 
 *
 * given a directory previously accessed and filled by the crawler and a file filled 
 * by the indexer, the query takes "queries" from the user through stdin and returns
 * a list of URLs that best match that query by a score calculated as the number of
 * occurrences of the given word conditions in those webpages according to the query.
 * This list will be ranked, starting with the URL with the highest score
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "index.h"
#include "word.h"
#include "pagedir.h"
#include "file.h"
#include "counters.h"
#include "memory.h"

/************* function prototypes ********************/

bool query(char* pageDirectory, char* indexFilename);
void processQuery(char* search, index_t* index, char* pageDirectory);
int countWordsInQuery(char* query);
char** parseQuery(char* query, int numWords);
void normalizeQuery(char** words, int numWords);
counters_t* getIDScores(char** words, int numWords, index_t* index, char* pageDirectory);

/************** main() ******************/
/* the "testing" function/main function, which takes two arguments 
 * as inputs (other than the executable call), the directory containing the
 * crawler directory and the name of the index file 
 * 
 * Pseudocode:
 *      1. Make sure there are exactly 2 arguments
 *      2. copy the pageDirectory and indexFilename into malloc'd strings
 *      3. validate the directory and indexFile
 *      4. call the querier method
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

    // make sure the directory is a valid crawler directory
    if (!pageDirValidate(pageDir)) {
        count_free(indexFilename);
        count_free(pageDir);
        fprintf(stderr, "Error: %s is an invalid crawler directory\n", pageDir);
        return false;
    }

    // check to see if the index file can be opened
    char* trueFilename = stringBuilder(NULL, indexFilename);
    if(fopen(trueFilename, "r") == NULL) {
        fprintf(stderr, "Error: provided filename %s is invalid\n", indexFilename);
    }

    // run the querier
    if (query(pageDir, indexFilename)) {
        printf("SUCCESS!\n\n");
        return 0;
    } else {
        printf("FAILED\n\n");
        return 1;
    }

}

/************** query() ******************/
/* The skeleton code for the query, which loads the index, 
 * reads from stdin with queries, and processes them
 * 
 * Pseudocode:
 *      1. load the index
 *      2. keep on taking from stdin while the query is active
 *      3. process those queries
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
*/
bool query(char* pageDirectory, char* indexFilename)
{
    FILE* fp = stdin;
    index_t* index = loadIndexFromFile(indexFilename);
    bool active = true;

    if(index != NULL) {

        while(active) {
            printf("Query: ");
            char* query = freadlinep(fp);
            if(query != NULL) {
                processQuery(query, index, pageDirectory);
            }
        }
    } else {
        return false;
    }
}

/************** processQuery() ******************/
/*
 *
 *
*/
void processQuery(char* query, index_t* index, char* pageDirectory) 
{
    int numWords = countWordsInQuery(query);

#ifdef TEST
        printf("there are %d nums\n", numWords);
#endif

    char** words = parseQuery(query, numWords);
    normalizeQuery(words, numWords);

    if(words == NULL) {
        return;
    }

#ifdef TEST
    for(int i = 0; i < numWords; i++) {
        char* word = *words;
        words++;
        printf("word %d: %s\n", i, word);
    }
#endif

    counters_t* idScores = getIDScores(words, numWords, index, pageDirectory);
    if(idScores == NULL) return;
    // begin reading through files and assigning scores
}

/************** countWordsInQuery() ******************/
/* count the number of words in a sentence */
int countWordsInQuery(char* query)
{
    int count = 0;
    bool lastSpace = true;
    for (char* i = query; *i != '\0'; i++) {
        if(isalpha(*i)) {
            if(lastSpace) count++;
            lastSpace = false;
        } else if (isspace(*i)) {
            lastSpace = true;
        }
    }

    return count;
}

/************** parseQuery() ******************/
/*
 *
 *
*/
char** parseQuery(char* query, int numWords)
{
    char** words = calloc(numWords, sizeof(char*));
    
    bool lastSpace = true;
    char** currWord = words;
    for (char* i = query; *i != '\0'; i++) {
        if(isalpha(*i)) {
            if(lastSpace) {
                *currWord = i;
                currWord++;
            } 
            lastSpace = false;

        } else if (isspace(*i)) {
            if(!lastSpace) *i = '\0';
            lastSpace = true;

        } else {
            fprintf(stderr, "Error: fsdjkfljdsljfl\n");
            return NULL;
        }
    }

    return words;
}

/************** normalizeQuery() ******************/
/* make a list of words all lowercase */
void normalizeQuery(char** words, int numWords) 
{
    if(words == NULL) return;
    for(int i = 0; i < numWords; i++) {
        char* word = *words;
        words++;
        normalizeWord(word);
    }

}

/************** getIDScores() ******************/
/*
 *
 *
*/
counters_t* getIDScores(char** words, int numWords, index_t* index, char* pageDirectory) 
{
    if(words == NULL || index == NULL || pageDirectory == NULL) return NULL;
    counters_t* idScores = counters_new();

}

