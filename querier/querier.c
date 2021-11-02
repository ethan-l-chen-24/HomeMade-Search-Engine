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
#include <unistd.h>
#include "index.h"
#include "word.h"
#include "pagedir.h"
#include "file.h"
#include "counters.h"
#include "memory.h"

/***************** local types ********************/

typedef struct countersTuple { // stores two counters to be passed to counters_iterate
    counters_t* counters1;
    counters_t* counters2;
} countersTuple_t;

typedef struct scoreID { // stores two ints, an id and its score for a query
    int docID;
    int score;
} scoreID_t;

typedef struct scoreIDArr { // stores an array of scoreIDs and keeps track of how much of the array has been filled
    scoreID_t** arr;
    int slotsFilled;
} scoreIDArr_t;

/************* function prototypes ********************/

// query methods
bool query(char* pageDirectory, char* indexFilename);
void processQuery(char* search, index_t* index, char* pageDirectory);
int countWordsInQuery(char* query);
char** parseQuery(char* query, int numWords);
void normalizeQuery(char** words, int numWords);

// scoring methods
counters_t* getIDScores(char** words, int numWords, index_t* index, char* pageDirectory);
bool orSequence(counters_t* prod, counters_t* scores);
counters_t* andSequence(counters_t* prod, counters_t* wordCount);
void countersUnionHelper(void* arg, const int key, const int count);
void countersIntersectionHelper(void* arg, const int key, const int count);

// ranking and printing methods
bool rankAndPrint(counters_t* idScores, char* pageDirectory);
void countFunc(void* arg, const int key, const int count);
void sortFunc(void* arg, const int key, const int count);

// struct deletion
void deleteScoreIDArr(scoreIDArr_t* scoreIDArr, int arrSize);

// Prompting
int fileno(FILE *stream);
void prompt(void);

// unit testing
#ifdef UNITTEST
    void unittest(void);
#endif

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
    // performs unit testing instead of regular querier if flag activated
    #ifdef UNITTEST
        unittest();
    #else

    char* program = argv[0];
    // check for the appropriate number of arguments
    if (argc != 3) {
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
        fprintf(stderr, "Error: %s is an invalid crawler directory\n", pageDir);
        count_free(indexFilename);
        count_free(pageDir);
        return 1;
    }

    // check to see if the index file can be opened
    char* trueFilename = stringBuilder2(NULL, indexFilename);
    FILE* fp;
    if ((fp = fopen(trueFilename, "r")) == NULL) {
        fprintf(stderr, "Error: provided filename %s is invalid\n", indexFilename);
        count_free(indexFilename);
        count_free(pageDir);
        count_free(trueFilename);
        return 1;
    }
    fclose(fp);
    count_free(trueFilename);

    // run the querier
    if (query(pageDir, indexFilename)) {
        #ifdef DEBUG
            printf("SUCCESS!\n\n");
        #endif
        return 0;
    } else {
        #ifdef DEBUG
            printf("FAILED\n\n");
        #endif
        return 1;
    }

    #endif
}

/************** query() ******************/
/* The skeleton code for the query, which loads the index, 
 * reads from stdin with queries, and processes them
 * 
 * Pseudocode:
 *      1. load the index
 *      2. keep on taking from stdin while the query is active
 *      3. process those queries
 *      4. continue until freadlinep notices EOF
 * 
 * Assumptions:
 *      1. the arguments are valid, otherwise throws errors
 *      2. assumes that the file at the indexFilename is in the valid format for loadIndexFromFile
*/
bool query(char* pageDirectory, char* indexFilename)
{
    // validate arguments
    if (pageDirectory == NULL || indexFilename == NULL) {
        if (pageDirectory != NULL) count_free(pageDirectory);
        if (indexFilename != NULL) count_free(indexFilename);
        fprintf(stderr, "Error: query failed");
        return false;
    }
    FILE* fp = stdin;
    // load in the hashtable from the index file
    index_t* index = loadIndexFromFile(indexFilename);

    if (index != NULL) {
        // prompt for user input
        prompt();
        char* query = freadlinep(fp);
        while(query != NULL) {
            // process the queries and ask again until EOF
            processQuery(query, index, pageDirectory);
            prompt();
            query = freadlinep(fp);
        }
        deleteIndex(index);
        count_free(pageDirectory);
        count_free(indexFilename);
        return true;
    } else {
        count_free(pageDirectory);
        count_free(indexFilename);
        return false;
    }
}

/************** processQuery() ******************/
/* parses the words, calculates the scores, and prints the documents using
 * other functions
 *
 * Pseudocode:
 *      1. count the number of words in the query
 *      2. parse the query into its words and make them all lowercase
 *      3. calculate the id scores based on the query words
 *      4. sort the final scores and print out the best queries
 * 
 * Assumptions:
 *      1. the arguments are valid, otherwise throws errors
 *      2. the page directory is a valid crawler directory
*/
void processQuery(char* query, index_t* index, char* pageDirectory) 
{
    // validate arguments
    if (query == NULL || index == NULL || pageDirectory == NULL) {
        fprintf(stderr, "Error: query failed");
        return;
    }

    // count the number of words in the query string
    int numWords = countWordsInQuery(query);

    #ifdef DEBUG
        printf("there are %d words\n", numWords);
    #endif

    // break that string into an array of words and make them lowercase
    char** words = parseQuery(query, numWords);
    if (words == NULL) return;
    normalizeQuery(words, numWords);

    #ifdef DEBUG // print all of the words
        char** wordTraverse = words;
        for (int i = 0; i < numWords; i++) {
            char* word = *wordTraverse;
            wordTraverse++;
            printf("word %d: %s\n", i, word);
        }
    #endif

    // calculate the stores
    counters_t* idScores = getIDScores(words, numWords, index, pageDirectory);
    count_free(query);
    count_free(words);
    if (idScores == NULL) return;
    
    // rank order the document ids by score and print them
    if (!rankAndPrint(idScores, pageDirectory)) return;
}

/************** countWordsInQuery() ******************/
/* count the number of words in a sentence */
int countWordsInQuery(char* query)
{
    // validate arguments
    if (query == NULL) return 0;
    int count = 0;
    bool lastSpace = true; // tracks if the last character read was a space

    // increment through each character until it is the end of the string
    for (char* i = query; *i != '\0'; i++) {
        // if it is a space, then the next letter is a new word
        if (isalpha(*i)) {
            if (lastSpace) count++;
            lastSpace = false;
        } else if (isspace(*i)) {
            lastSpace = true;
        }
    }

    return count;
}

/************* parseQuery() ***************/
/* takes the query string and splits it into an array of words.
 * it splits by spaces, and any bad characters will lead to a null
 * return
 *
 * Pseudocode: 
 *      1. allocate space for the word array
 *      2. iterate through the characters in the query
 *      3. for each, check if it is a letter, and if it was preceded
 *          by a space, make that pointer a pointer in the word array
 *      4. if you find a space and the last was a letter, insert the '\0' 
 *          character to break the word from the rest of the string
 *      5. if a null character is read, throw an error
 *      6. return the word array
 * 
 * Assumptions:
 *      1. The arguments are valid, otherwise throw error
 *      2. all of the characters in the query are understandable by ctype.h
*/
char** parseQuery(char* query, int numWords)
{
    // validate arguments
    if (query == NULL || numWords == 0) {
        if (query != NULL) count_free(query);
        return NULL;
    }
   
    char** words = count_calloc(numWords, sizeof(char*));
    if (words == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        count_free(query);
        return NULL;
    }

    bool lastSpace = true; // tracks if the last character read was a space
    char** currWord = words; 

    // iterate through every character until at the end of the query string
    for (char* i = query; *i != '\0'; i++) {
        // if it is a letter preceded by a space, it is the start of a new word
        // set the array value to this pointer and then increment the array pointer
        if (isalpha(*i)) { 
            if (lastSpace) {
                *currWord = i;
                currWord++;
            } 
            lastSpace = false;

        // if it is a space preceded by a letter, then it is the end of a string,
        // so you should insert '\0' to terminate that string
        } else if (isspace(*i)) {
            if (!lastSpace) *i = '\0';
            lastSpace = true;

        // throw an error if a bad char is read.
        } else {
            fprintf(stderr, "Error: bad character '%c' in query\n", *i);
            count_free(words);
            count_free(query);
            return NULL;
        }
    }

    return words;
}

/************** normalizeQuery() ******************/
/* make a list of words all lowercase */
void normalizeQuery(char** words, int numWords) 
{
    // validate arguments
    if (words == NULL) return;

    // traverse through the words in words
    char** wordTraverse = words;
    for (int i = 0; i < numWords; i++) {
        char* word = *wordTraverse;
        if (word != NULL) normalizeWord(word); // make them lowercase
        wordTraverse++;
    }

}

/************** getIDScores() ******************/
/* given the parsed words, the index, and additional necessary arguments,
 * this method calculates the scores of different documents with respect to the
 * query
 *
 * Pseudocode: 
 *      1. initialize the structs
 *      2. loop through all of the words in the query
 *      3. check if the word is an operator or a query word
 *      4. if it is an 'and', check for errors and then ignore
 *      5. if it is an 'or', check for errors and then run an orsequence to merge the running product
 *          with the scores
 *      6. if it is a word, and first in the sequence merge the product with the current word's counters, 
 *          otherwise treat it as an and and intersect the countersets with an andsequence
 *      7. At the end of the words, perform a final merge
 * 
 * Assumptions:
 *      1. The arguments are valid, otherwise throw errors
*/
counters_t* getIDScores(char** words, int numWords, index_t* index, char* pageDirectory) 
{
    // validate args
    if (words == NULL || index == NULL || pageDirectory == NULL) {
        return NULL;
    }

    // initialize structs
    counters_t* prod = counters_new();
    counters_t* scores = counters_new();
    hashtable_t* indexTable = getHashtable(index);

    bool firstInSequence = true; // if the next word is the first in an and sequence
    char* lastWord = ""; // initialized so we know it is the beginning of the query
    char** wordTraverse = words;
    // traverse through all of the words in the query
    for (int i = 0; i < numWords; i++) {
        char* word = *wordTraverse;
        wordTraverse++;

        // if and, edge cases throw errors, otherwise continue to next word
        if (strcmp(word, "and") == 0) {
            #ifdef DEBUG
                printf("AND SEQUENCE\n---------------\n");
            #endif
            if (strcmp(lastWord, "") == 0 || strcmp(lastWord, "or") == 0 || strcmp(lastWord, "and") == 0) {
                if (strcmp(lastWord, "") == 0) {
                    fprintf(stderr, "Error: 'and' cannot be first\n");
                    counters_delete(prod);
                    counters_delete(scores);
                    return NULL;
                } else {
                    fprintf(stderr, "Error: '%s' and 'and' cannot be adjacent\n", lastWord);
                    counters_delete(prod);
                    counters_delete(scores);
                    return NULL;
                }
            }
            lastWord = word;
            continue;

        // if or, edge cases throw errors, otherwise run an or sequence and reset the running product
        } else if (strcmp(word, "or") == 0) {
            #ifdef DEBUG
                printf("OR SEQUENCE\n--------------\n");
            #endif
            if (strcmp(lastWord, "") == 0 || strcmp(lastWord, "or") == 0 || strcmp(lastWord, "and") == 0) {
                if (strcmp(lastWord, "") == 0) {
                    fprintf(stderr, "Error: 'or' cannot be first\n");
                    counters_delete(prod);
                    counters_delete(scores);
                    return NULL;
                } else {
                    fprintf(stderr, "Error: '%s' and 'or' cannot be adjacent\n", lastWord);
                    counters_delete(prod);
                    counters_delete(scores);
                    return NULL;
                }
            }
            orSequence(prod, scores); // run the or
            counters_delete(prod);
            prod = counters_new(); // reset the prod
            firstInSequence = true;

        // if an actual word is read, merge the prod with the word's counter if the first in the sequence
        // otherwise run an and sequence
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
        lastWord = word; // increment the last word
    }
    // check for edge cases
    if (strcmp(lastWord, "or") == 0 || strcmp(lastWord, "and") == 0) {
        fprintf(stderr, "Error: '%s' cannot be last\n", lastWord);
        counters_delete(prod);
        counters_delete(scores);
        return NULL;
    } else if (strcmp(lastWord, "") == 0) {
        counters_delete(prod);
        counters_delete(scores);
        return NULL;
    } else {
        // merge the final product with the scores
        orSequence(prod, scores);
        counters_delete(prod);
        return scores;
    }
}

/************** orSequence() ******************/
/* runs an orSequence, which merges prod into scores */
bool orSequence(counters_t* prod, counters_t* scores) 
{
    if (prod == NULL || scores == NULL) return false;
    #ifdef DEBUG
        printf("Last prod: ");
        counters_print(prod, stdout);
        printf("\n");
        printf("Current score: ");
        counters_print(scores, stdout);
        printf("\n\n");
    #endif

    // iterate through prod and add to scores
    counters_iterate(prod, scores, countersUnionHelper);

    #ifdef DEBUG
        printf("Scores after union: ");
        counters_print(scores, stdout);
        printf("\n");
    #endif

    return true;
}

/************** andSequence() ******************/
/* runs an andSequence, which finds the intersection between countersets.
 * In the process, it creates a third temporary counterset to store the intersection
 * before it sets the new one equal to the prod
 *
 * Pseudocode:
 *      1. create a new counters
 *      2. add that and the prod counters to the tuple
 *      3. iterate through wordcount, checking for intersecting words in prod and add
 *          them to intersection
 * 
 *  Assumptions:
 *      1. The arguments are valid, otherwise throw errors
*/
counters_t* andSequence(counters_t* prod, counters_t* wordCount)
{
    // validate arguments
    if (prod == NULL || wordCount == NULL) {
        if (prod != NULL) counters_delete(prod);
        if (wordCount != NULL) counters_delete(wordCount);
        return NULL;
    }

    #ifdef DEBUG
        printf("New word: ");
        counters_print(wordCount, stdout);
        printf("\n");
        printf("Current prod: ");
        counters_print(prod, stdout);
        printf("\n\n");
    #endif

    // create a new counterset
    counters_t* intersection = counters_new();
    if (intersection == NULL) return false;

    // create a new tuple
    countersTuple_t* tuple = count_malloc(sizeof(countersTuple_t));
    if (tuple == NULL) return false;
    tuple->counters1 = prod;
    tuple->counters2 = intersection;

    // pass the tuple and the other set to the iterate method
    counters_iterate(wordCount, tuple, countersIntersectionHelper);

    #ifdef DEBUG
        printf("Prod after intersection: ");
        counters_print(intersection, stdout);
        printf("\n");
    #endif 

    counters_delete(prod);
    count_free(tuple);
    return intersection;
}

/************** countersUnionHelper() ******************/
/* merges two counters together by adding their counts together and setting the key 
 * to the value
 */
void countersUnionHelper(void* arg, const int key, const int count) 
{  
    // validate arguments
    if (arg == NULL) return;
    // get the counter, find the count in the other counterset, and add 
    // to that counterset the count of the counterset being iterated through
    counters_t* ctr1 = arg;
    int count1 = counters_get(ctr1, key);
    counters_set(ctr1, key, count1 + count);
}

/************** countersIntersectionHelper() ******************/
/* Finds the intersection between two countersets by finding the minimum of both
 * of their counts for a key and adding the min to the other counterset given in the tuple
*/
void countersIntersectionHelper(void* arg, const int key, const int count) 
{
    // validate arguments
    if (arg == NULL) return;
    countersTuple_t* tuple = arg;
    // find the count of the key in the other counters
    int count1 = counters_get(tuple->counters1, key);
    int intersectionScore;
    // get the min score
    if (count1 < count) intersectionScore = count1;
    else intersectionScore = count;
    // as long as it isn't zero, add it to the intersection set
    if (intersectionScore != 0) counters_set(tuple->counters2, key, intersectionScore);
}

/************** rankAndPrint() ******************/
/* given the scores in a counterset, sort them by score and print the corresponding
 * ID numbers, scores, and URLs
 *
 * Pseudocode:
 *      1. iterate to count how many scores are in idScores
 *      2. allocate enough memory in an array for pointers to a score and ID struct
 *      3. iterate through and populate the array with the correctly sorted ID-score structs
 *      4. open the crawler file of each ID and grab the URL
 *      4. print out the score, doc ID, and HTML
 * 
 * Assumptions:
 *      1. The arguments are valid, otherwise throw errors
 *      2. the pageDirectory is a valid crawler directory
*/
bool rankAndPrint(counters_t* idScores, char* pageDirectory)
{
    // validate arguments
    if (idScores == NULL || pageDirectory == NULL) return false;

    // count the number of ids that satisfied the query
    int count = 0;
    counters_iterate(idScores, &count, countFunc);
    #ifdef DEBUG
        printf("there are %d valid options\n", count);
    #endif

    if (count == 0) {
        printf("No documents match.\n");
        counters_delete(idScores);
        return true;
    } else {
        // allocate enough space for the array of structs
        scoreID_t** arr = count_calloc(count, sizeof(scoreID_t*));
        if (arr == NULL) {
            fprintf(stderr, "Error: out of memory\n");
            return false;
        }
        // create the struct that stores the array
        scoreIDArr_t* scoreIDArr = count_malloc(sizeof(scoreIDArr_t));
        if (scoreIDArr == NULL) {
            fprintf(stderr, "Error: out of memory\n");
            count_free(arr);
            return false;
        }
        scoreIDArr->arr = arr;
        scoreIDArr->slotsFilled = 0;
        // sort the id-scores into the array
        counters_iterate(idScores, scoreIDArr, sortFunc);
        counters_delete(idScores);

        #ifdef DEBUG
            for(int i = 0; i<count; i++) {
                printf("Index %d: docID - %d, score - %d\n", i, arr[i]->docID, arr[i]->score);
            }
        #endif

        // loop through all of the items in the array
        for (int i = 0; i<count; i++) {
            int id = arr[i]->docID;
            int score = arr[i]->score;
            char* idString = intToString(id); // build the filepath
            if (idString == NULL) continue;

            // open the filepath to retrieve the URL
            char* filepath = stringBuilder2(pageDirectory, idString);
            count_free(idString);
            if (filepath == NULL) continue;
            FILE* fp = fopen(filepath, "r");
            count_free(filepath);
            if (fp != NULL) {
                char* URL = freadlinep(fp);
                if (URL == NULL) {
                    fclose(fp);
                    continue;
                }
                // print out the score line
                printf("score %3d doc %3d: %s\n", score, id, URL);
                count_free(URL);
                fclose(fp);
            }
        }

        deleteScoreIDArr(scoreIDArr, count);
    }
    // print a looooooong bar
    printf("-----------------------------------------------------------------------------\n");
    return true;
}

/************** countFunc() ******************/
/* helper that counts the number of items in the counterset */
void countFunc(void* arg, const int key, const int count)
{
    int* num = arg;
    (*num)++; // increment the count
}

/************** sortFunc() ******************/
/* implements a modified insertionSort to insert key-count structs into the 
 * array, passed as the void* arg. It finds its location in the array and shifts 
 * everything else one spot to the right until it is completely filled.
 *
 * Pseudocode:
 *      1. get our array from the void* arg
 *      2. loop through all of the filled items in the array until the current
 *          item is larger than the item in the array
 *      3. store the currently-stored struct in a temp variable and replace it 
 *          with the new score-id struct
 *      4. until the temp is null aka until all of the items have been shifted, shift
 *          the items one to the right, always saving a second temp to ensure no value is lost
 *      5. if there is never a larger item, it is assumed the smallest and placed at the end at 
 *          the same index as slotsFilled
 *      6. slotsFilled is incremented
 * 
 * Assumptions:
 *      1. The arguments are valid, otherwise throw errors
 *      2. the arg is actually of type scoreIDArr_t*
 *      3. the array has enough spots to hold the next key and count
*/
void sortFunc(void* arg, const int key, const int count)
{
    // validate arguments
    if (arg == NULL) return;

    #ifdef DEBUG
        printf("SORTING ID %d, COUNT %d\n", key, count);
    #endif

    // get the array from the arg argument
    scoreIDArr_t* scoreIDArr = arg;
    scoreID_t** arr = scoreIDArr->arr;
    int slotsFilled = scoreIDArr->slotsFilled;

    // create a new struct and initialize its key and count
    scoreID_t* newScoreID = count_malloc(sizeof(scoreID_t));
    if (newScoreID == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        return;
    }
    newScoreID->docID = key;
    newScoreID->score = count;

    // loop through all of the existing id-score pairs in the array
    for (int i = 0; i < slotsFilled; i++) {
        // once it is larger, shift everything else to the right
        if (count > arr[i]->score) {
            int j = 1; // used to shift items further and further away
            scoreID_t* temp = arr[i]; // a temp variable to not lose the value currently stored
            arr[i] = newScoreID;
            while (temp != NULL) {
                scoreID_t* temp2 = arr[i + j]; // a temp variable to not lose the value stored
                arr[i + j] = temp;
                temp = temp2;
                j++; // move ahead
            }

            scoreIDArr->slotsFilled++;
            return; // exit once everything has shifted - no need to check anything else
        }
    }
    // case where it is the smallest item, insert it at end and increment slotsFilled
    arr[slotsFilled] = newScoreID;
    scoreIDArr->slotsFilled++;
}

/************** prompt() ******************/
/* prompts the user for input if stdin is used */
void prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/************** deleteScoreIDArr() ******************/
/* deletes the scoreIDArr struct */
void deleteScoreIDArr(scoreIDArr_t* scoreIDArr, int arrSize) 
{
    if (scoreIDArr == NULL) return;
    scoreID_t** arr = scoreIDArr->arr; // get the array
    if (arr == NULL) {
        count_free(scoreIDArr);
        return;
    }
    for (int i = 0; i<arrSize; i++) { // loop through items inside and free
        if (arr[i] != NULL) count_free(arr[i]);
    }
    count_free(arr); // free the array
    count_free(scoreIDArr); // free the wrapper struct
}


/*********************** UNIT TESTING **************************/

#ifdef UNITTEST

    // unit testing for the countWordsInQuery function 
    int test1()
    {
        int numFailed = 0;
        char* query = "there are seven words in this sentence";
        char* query2 = "     this      is ....a     crazy *&( input";
        char* query3 = "a\nb";
        int numWords = countWordsInQuery(query);
        int numWords2 = countWordsInQuery(query2);
        int numWords3 = countWordsInQuery(query3);
        int numWords4 = countWordsInQuery(NULL);

        // check if correct counts
        if (numWords != 7) numFailed++;
        if (numWords2 != 5) numFailed++;
        if (numWords3 != 2) numFailed++;
        if (numWords4 != 0) numFailed++;

        return numFailed;
    }

    // unit testing for the parseQuery function
    int test2()
    {
        int numFailed = 0;
        // building a non-final string
        char* q = "there are seven words in this sentence";
        char* q2 = "     there     are   multiple spacesss     in this";
        char* q3 = "  there are special cha&acters in this sentence";

        char* query = malloc(strlen(q) + 1);
        char* query2 = malloc(strlen(q2) + 1);
        char* query3 = malloc(strlen(q3) + 1);
        strcpy(query, q);
        strcpy(query2, q2);
        strcpy(query3, q3);

        int numWords = countWordsInQuery(query);
        char** pq = parseQuery(query, numWords);
        int numWords2 = countWordsInQuery(query2);
        char** pq2 = parseQuery(query2, numWords2);
        int numWords3 = countWordsInQuery(query3);
        char** pq3 = parseQuery(query3, numWords3);
        char** pq4 = parseQuery(NULL, 0);

        // check if the words were parsed correctly
        if (strcmp(pq[0], "there") != 0) numFailed++;
        if (strcmp(pq[6], "sentence") != 0) numFailed++;
        if (strcmp(pq2[0], "there") != 0) numFailed++;
        if (strcmp(pq2[4], "in") != 0) numFailed++;
        if (pq3 != NULL) numFailed++;
        if (pq4 != NULL) numFailed++;

        // frees
        count_free(query);
        count_free(query2);
        count_free(pq);
        count_free(pq2);

        return numFailed;
    }

    // unit testing for the normalizeQuery function
    int test3()
    {
        int numFailed = 0;
        char* q = "THeRe aRE SevEN WoRDS iN ThIs sENTENcE";

        char* query = malloc(strlen(q) + 1);
        strcpy(query, q);

        int numWords = countWordsInQuery(query);
        char** pq = parseQuery(query, numWords);
        char** pq2 = NULL;
        normalizeQuery(pq, numWords);
        normalizeQuery(pq2, 0);

        // check if the words all became lowercase
        if (strcmp(pq[0], "there") != 0) numFailed++;
        if (strcmp(pq[3], "words") != 0) numFailed++;
        if (strcmp(pq[4], "in") != 0) numFailed++;
        if (strcmp(pq[6], "sentence") != 0) numFailed++;
        if (pq2 != NULL) numFailed++;

        // frees
        count_free(query);
        count_free(pq);

        return numFailed;
    }

    // unit testing for the orSequence function
    int test4()
    {
        int numFailed = 0; 
        counters_t* c1 = counters_new();
        counters_set(c1, 1, 5);
        counters_set(c1, 2, 4);
        counters_t* c2 = counters_new();
        counters_set(c2, 1, 6);
        counters_set(c2, 3, 6);
        counters_t* c3 = counters_new();
        counters_t* c4 = NULL;

        // check if the sets merged successfully
        if (!orSequence(c2, c1)) numFailed++;
        if (counters_get(c1, 1) != 11) numFailed++;
        if (counters_get(c1, 2) != 4) numFailed++;
        if (counters_get(c1, 3) != 6) numFailed++;
        if (counters_get(c2, 1) != 6) numFailed++;
        if (counters_get(c2, 2) != 0) numFailed++;

        if (!orSequence(c3, c1)) numFailed++;
        if (counters_get(c3, 1) != 0) numFailed++;
        if (counters_get(c3, 2) != 0) numFailed++;

        if (!orSequence(c1, c3)) numFailed++;
        if (counters_get(c3, 1) != 11) numFailed++;
        if (counters_get(c3, 2) != 4) numFailed++;

        if (orSequence(c4, c3)) numFailed++;
        if (counters_get(c3, 1) != 11) numFailed++;

        // frees
        counters_delete(c1);
        counters_delete(c2);
        counters_delete(c3);

        return numFailed;
    }

    // unit testing for the andSequence function
    int test5()
    {
        int numFailed = 0; 
        counters_t* c1 = counters_new();
        counters_set(c1, 1, 5);
        counters_set(c1, 2, 4);
        counters_set(c1, 3, 6);
        counters_t* c2 = counters_new();
        counters_set(c2, 2, 6);
        counters_set(c2, 3, 1);
        counters_set(c2, 4, 5);
        counters_t* c3 = counters_new();
        counters_t* c4 = NULL;

        // check if the sets intersect properly
        counters_t* temp1 = andSequence(c1, c2);
        if (counters_get(temp1, 1) != 0) numFailed++;
        if (counters_get(temp1, 2) != 4) numFailed++;
        if (counters_get(temp1, 3) != 1) numFailed++;
        if (counters_get(c2, 2) != 6) numFailed++;
        if (counters_get(c2, 4) != 5) numFailed++;

        counters_t* temp2 = andSequence(temp1, c3);
        if (counters_get(temp2, 1) != 0) numFailed++;
        if (counters_get(temp2, 2) != 0) numFailed++;

        counters_t* temp3 = andSequence(temp2, c4);
        if (temp3 != NULL) numFailed++;

        // frees
        counters_delete(temp3);
        counters_delete(c2);
        counters_delete(c3);

        return numFailed;
    }

    // unit testing for the countFunc function
    int test6()
    {
        int numFailed = 0;
        counters_t* counters = counters_new();
        for (int i = 0; i < 10; i++) {
            counters_add(counters, i);
        }
        int count = 0;
        counters_iterate(counters, &count, countFunc);

        counters_t* counters2 = counters_new();
        int count2 = 0;
        counters_iterate(counters2, &count2, countFunc);

        // check if the function counts properly
        if (count != 10) numFailed++;
        if (count2 != 0) numFailed++;

        // frees
        counters_delete(counters);
        counters_delete(counters2);

        return numFailed;
    }

    // unit testing for the countFunc function
    int test7()
    {
        int numFailed = 0;

        // ascending order test
        scoreIDArr_t* arrObj = count_malloc(sizeof(scoreIDArr_t));
        scoreID_t** arr = count_calloc(10, sizeof(scoreID_t*));
        arrObj->arr = arr;
        arrObj->slotsFilled = 0;
        counters_t* counters = counters_new();
        for (int i = 1; i <= 10; i++) {
            counters_set(counters, i, i);
        }
        counters_iterate(counters, arrObj, sortFunc);

        // descending order test
        scoreIDArr_t* arrObj2 = count_malloc(sizeof(scoreIDArr_t));
        scoreID_t** arr2 = count_calloc(10, sizeof(scoreID_t*));
        arrObj2->arr = arr2;
        arrObj2->slotsFilled = 0;
        counters_t* counters2 = counters_new();
        for (int i = 10; i > 0; i--) {
            counters_set(counters2, i, i);
        }
        counters_iterate(counters2, arrObj2, sortFunc);

        // non-sorted order test
        scoreIDArr_t* arrObj3 = count_malloc(sizeof(scoreIDArr_t));
        scoreID_t** arr3 = count_calloc(10, sizeof(scoreID_t*));
        arrObj3->arr = arr3;
        arrObj3->slotsFilled = 0;
        counters_t* counters3 = counters_new();
        for (int i = 1; i <= 10; i++) {
            counters_set(counters3, i, (i*i)%10);
        }
        counters_iterate(counters3, arrObj3, sortFunc);

        // check if the arrays are sorted properly
        for (int i = 0; i < 10; i++) {
            if (arr[i]->docID != (10 - i)) {
                numFailed++;
            }
        }
        for (int i = 0; i < 10; i++) {
            if (arr2[i]->docID != (10 - i)) {
                numFailed++;
            }
        }

        // check if the average-case works (I figured)
        // these out by hand
        if (arr3[9]->docID != 10) numFailed++;
        if (arr3[4]->docID != 5) numFailed++;
        if (arr3[3]->docID != 6) numFailed++;

        // frees
        for (int i = 0; i < 10; i++) {
            count_free(arr[i]);
            count_free(arr2[i]);
            count_free(arr3[i]);
        }
        count_free(arr);
        count_free(arr2);
        count_free(arr3);
        count_free(arrObj);
        count_free(arrObj2);
        count_free(arrObj3);
        counters_delete(counters);
        counters_delete(counters2);
        counters_delete(counters3);
        
        return numFailed;
    }
    
    // runs the unit testing, called in main above
    void unittest() 
    {
        int totalFailed = 0;

        // test 1: countWordsInQuery
        int failed = 0;
        printf("Welcome to Unit Testing\n");
        failed += test1();
        if (failed == 0) {
            printf("Test 1 passed\n");
        } else {
            printf("Test 1 failed!\n");
            totalFailed++;
        }

        // test 2: parseQuery
        failed = 0;
        failed += test2();
        if (failed == 0) {
            printf("Test 2 passed\n");
        } else {
            printf("Test 2 failed!\n");
            totalFailed++;
        }

        // test 3: normalizeQuery
        failed = 0;
        failed += test3();
        if (failed == 0) {
            printf("Test 3 passed\n");
        } else {
            printf("Test 3 failed!\n");
            totalFailed++;
        }

        // test 4: orSequence
        failed = 0;
        failed += test4();
        if (failed == 0) {
            printf("Test 4 passed\n");
        } else {
            printf("Test 4 failed!\n");
            totalFailed++;
        }

        // test 5: andSequence
        failed = 0;
        failed += test5();
        if (failed == 0) {
            printf("Test 5 passed\n");
        } else {
            printf("Test 5 failed!\n");
            totalFailed++;
        }

        // test 6: countFunc
        failed = 0;
        failed += test6();
        if (failed == 0) {
            printf("Test 6 passed\n");
        } else {
            printf("Test 6 failed!\n");
            totalFailed++;
        }

        // test 7: sortFunc
        failed = 0;
        failed += test7();
        if (failed == 0) {
            printf("Test 7 passed\n");
        } else {
            printf("Test 7 failed!\n");
            totalFailed++;
        }
    }

#endif