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

bool orSequence(counters_t* prod, counters_t* scores);
counters_t* andSequence(counters_t* prod, counters_t* wordCount);
static void countersUnionHelper(void* arg, const int key, const int count);
static void countersIntersectionHelper(void* arg, const int key, const int count);

static void rankAndPrint(counters_t* idScores, char* pageDirectory);
static void countFunc(void* arg, const int key, const int count);
static void sortFunc(void* arg, const int key, const int count);

typedef struct countersTuple {
    counters_t* counters1;
    counters_t* counters2;
} countersTuple_t;

typedef struct scoreID {
    int docID;
    int score;
} scoreID_t;

typedef struct scoreIDArr {
    scoreID_t** arr;
    int size;
    int slotsFilled;
} scoreIDArr_t;

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

    if(index != NULL) {
        printf("Query: ");
        char* query = freadlinep(fp);
        while(query != NULL) {
            processQuery(query, index, pageDirectory);
            printf("Query: ");
            query = freadlinep(fp);
        }
        return true;
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

#ifdef DEBUG
        printf("there are %d words\n", numWords);
#endif

    char** words = parseQuery(query, numWords);
    normalizeQuery(words, numWords);

    if(words == NULL) {
        return;
    }

#ifdef DEBUG
    char** wordTraverse = words;
    for(int i = 0; i < numWords; i++) {
        char* word = *wordTraverse;
        wordTraverse++;
        printf("word %d: %s\n", i, word);
    }
#endif

    counters_t* idScores = getIDScores(words, numWords, index, pageDirectory);
    if(idScores == NULL) return;
    
    rankAndPrint(idScores, pageDirectory);
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
    char** words = count_calloc(numWords, sizeof(char*));
    
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
            fprintf(stderr, "Error: bad character '%c' in query\n", *i);
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
    char** wordTraverse = words;
    for(int i = 0; i < numWords; i++) {
        char* word = *wordTraverse;
        normalizeWord(word);
        wordTraverse++;
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
    counters_t* prod = counters_new();
    counters_t* scores = counters_new();
    hashtable_t* indexTable = getHashtable(index);

    bool firstInSequence = true;
    char* lastWord = "";
    char** wordTraverse = words;
    for(int i = 0; i < numWords; i++) {
        char* word = *wordTraverse;
        wordTraverse++;

        if(strcmp(word, "and") == 0) {
            #ifdef DEBUG
                printf("AND SEQUENCE\n---------------\n");
            #endif
            if(strcmp(lastWord, "") == 0 || strcmp(lastWord, "or") == 0 || strcmp(lastWord, "and") == 0) {
                if(strcmp(lastWord, "") == 0) {
                    fprintf(stderr, "Error: 'and' cannot be first\n");
                    return NULL;
                } else {
                    fprintf(stderr, "Error: '%s' and 'and' cannot be adjacent\n", lastWord);
                    return NULL;
                }
            }
            lastWord = word;
            continue;
        } else if(strcmp(word, "or") == 0) {
            #ifdef DEBUG
                printf("OR SEQUENCE\n--------------\n");
            #endif
            if(strcmp(lastWord, "") == 0 || strcmp(lastWord, "or") == 0 || strcmp(lastWord, "and") == 0) {
                if(strcmp(lastWord, "") == 0) {
                    fprintf(stderr, "Error: 'or' cannot be first\n");
                    return NULL;
                } else {
                    fprintf(stderr, "Error: '%s' and 'or' cannot be adjacent\n", lastWord);
                    return NULL;
                }
            }
            orSequence(prod, scores);
            counters_delete(prod);
            prod = counters_new();
            firstInSequence = true;
        } else {
            counters_t* indexCounter = hashtable_find(indexTable, word);
            #ifdef DEBUG 
                printf("\nFOUND WORD %s\n\n", word); 
            #endif
            if (firstInSequence) {
                #ifdef DEBUG
                    printf("OR SEQUENCE\n---------------\n");
                #endif
                orSequence(indexCounter, prod);
            } else {
                #ifdef DEBUG
                    printf("AND SEQUENCE\n---------------\n");
                #endif
                prod = andSequence(prod, indexCounter);
            }
            firstInSequence = false;
        }
        lastWord = word;
    }
    if(strcmp(lastWord, "or") == 0 || strcmp(lastWord, "and") == 0) {
        fprintf(stderr, "Error: '%s' cannot be last\n", lastWord);
        return NULL;
    } else {
        orSequence(prod, scores);
        return scores;
    }
}

bool orSequence(counters_t* prod, counters_t* scores) 
{
#ifdef DEBUG
    printf("Last prod: ");
    counters_print(prod, stdout);
    printf("\n");
    printf("Current score: ");
    counters_print(scores, stdout);
    printf("\n\n");
#endif

    counters_iterate(prod, scores, countersUnionHelper);

#ifdef DEBUG
    printf("Scores after union: ");
    counters_print(scores, stdout);
    printf("\n");
#endif

    return true;
}

counters_t* andSequence(counters_t* prod, counters_t* wordCount)
{
#ifdef DEBUG
    printf("New word: ");
    counters_print(wordCount, stdout);
    printf("\n");
    printf("Current prod: ");
    counters_print(prod, stdout);
    printf("\n\n");
#endif

    counters_t* intersection = counters_new();
    if(intersection == NULL) return false;

    countersTuple_t* tuple = count_malloc(sizeof(countersTuple_t));
    if(tuple == NULL) return false;
    tuple->counters1 = prod;
    tuple->counters2 = intersection;

    counters_iterate(wordCount, tuple, countersIntersectionHelper);

#ifdef DEBUG
    printf("Prod after intersection: ");
    counters_print(intersection, stdout);
    printf("\n");
#endif 

    counters_delete(prod);
    return intersection;
}

static void countersUnionHelper(void* arg, const int key, const int count) 
{
    counters_t* ctr1 = arg;
    int count1 = counters_get(ctr1, key);
    counters_set(ctr1, key, count1 + count);
}

static void countersIntersectionHelper(void* arg, const int key, const int count) 
{
    countersTuple_t* tuple = arg;
    int count1 = counters_get(tuple->counters1, key);
    int intersectionScore;
    if (count1 < count) intersectionScore = count1;
    else intersectionScore = count;
    if(intersectionScore != 0) {
        counters_add(tuple->counters2, key);
        counters_set(tuple->counters2, key, intersectionScore);
    }
}

static void rankAndPrint(counters_t* idScores, char* pageDirectory)
{
    int count = 0;
    counters_iterate(idScores, &count, countFunc);
    #ifdef DEBUG
        printf("there are %d valid options\n", count);
    #endif

    if(count == 0) {
        printf("No documents match.\n");
    } else {

        scoreID_t** arr = count_calloc(count, sizeof(scoreID_t*));
        scoreIDArr_t* scoreIDArr = count_malloc(sizeof(scoreIDArr_t));
        scoreIDArr->arr = arr;
        scoreIDArr->size = count;
        scoreIDArr->slotsFilled = 0;
        counters_iterate(idScores, scoreIDArr, sortFunc);

    #ifdef DEBUG
        for(int i = 0; i<scoreIDArr->size; i++) {
            printf("Index %d: docID - %d, score - %d\n", i, arr[i]->docID, arr[i]->score);
        }
    #endif

        for(int i = 0; i<count; i++) {
            int id = arr[i]->docID;
            int score = arr[i]->score;
            char* idString = intToString(id);
            char* filepath = stringBuilder(pageDirectory, idString);
            FILE* fp = fopen(filepath, "r");
            if(fp != NULL) {
                char* URL = freadlinep(fp);
                printf("score %3d doc %3d: %s\n", score, id, URL);
            }
        }
    }
    printf("-----------------------------------------------------------------------------\n");

}

static void countFunc(void* arg, const int key, const int count)
{
    int* num = arg;
    (*num)++;
}

static void sortFunc(void* arg, const int key, const int count)
{
    #ifdef DEBUG
        printf("SORTING ID %d, COUNT %d\n", key, count);
    #endif

    scoreIDArr_t* scoreIDArr = arg;
    scoreID_t** arr = scoreIDArr->arr;
    int slotsFilled = scoreIDArr->slotsFilled;

    scoreID_t* newScoreID = count_malloc(sizeof(scoreID_t));
    newScoreID->docID = key;
    newScoreID->score = count;

    for(int i = 0; i < slotsFilled; i++) {
        if (count > arr[i]->score) {
            int j = 1;
            scoreID_t* temp = arr[i];
            arr[i] = newScoreID;
            while(temp != NULL) {
                scoreID_t* temp2 = arr[i + j];
                arr[i + j] = temp;
                temp = temp2;
                j++;
            }

            scoreIDArr->slotsFilled++;
            return;
        }

    }
}

