# Lab 5 - Ethan Chen
# CS50 Fall 2021

### Implementation

The indexer is implemented in _C_ using _VSCode_ as an editor. It was also developed using MacOS.

The indexer is implemented with respect to the design specs in `DESIGN.md`.

The algorithm follows the pseudocode as described in `DESIGN.md`. Here is the major data flow and pseudocode for all of the modules, as well as any additional necessary helper functions.

#### `main`
Runs the indexer

1. validate args
2. call pageDirValidate, and if false, return
3. call indexer

#### `pageDirValidate`
Makes sure a pageDir is a valid crawler directory

1. build the filepath of the .crawler file
2. try to read the file
3. if it is possible, return true, otherwise return false

#### `indexer`
Builds the index and then prints it out

1. create a new index struct
2. call buildIndexFromCrawler
3. call saveIndexToFile

#### `buildIndexFromCrawler`
inserts items into the index from a crawler directory

1. sets the first id = 1
2. goes through each webpage until it can't read anymore
    1. tries to index the webpage by calling indexWebpage, which will also increment the id by calling loadPageToWebpage

#### `indexWebpage`
this method really just calls readWordsInFile, and is not worthy of legitimate pseudocode

#### `readWordsInWebpage`
goes through all of the words in a webpage and loads them into the index

1. get words from the webpage as long as they are not null
    1. make the word lowercase
    2. check if the word is more than 2 characters
    3. find the counterset of that word in the index
    4. if it doesn't exist
        1. add a new counter set in the hashtable
        2. add the word to it
    5. if it does exist
        1. get the counter set and add the word
2. increment the id

#### `loadPageToWebpage`
takes a pagedirectory and id of a crawler page, retrieves the URL and builds the webpage

1. builds the filepath of the crawler file
2. tries to open the file
    1. if possible, read the first line
    2. check if the URL is internal/valid
    3. create a new webpage with the URL as its URL
    4. Try to get the URL of the webpage
    5. Return the page

#### `saveIndexToFile`
takes an index  and saves it to a file

1. builds the filepath of the output file
2. tries to open that file
    1. iterates through and prints, using the printCT function we developed in class

The `indextest` is not quite as worthy of mention, as it is meant as a tester for the `index` file. However, there are a couple of methods worthy of mention that will be relevant for the `query` module.

#### `loadIndexFromFile`
Loads the index struct from an index output file

1. create a new index
2. build the filepath of the index file
3. open the file and read from it
4. read a word as long as it exists
5. call loadWordInIndex and pass that word
6. close memory

#### `loadWordInIndex`
Loads a specific word's ids and frequency into the index

1. create a new counterset
2. try to insert it into the index
3. read pairs of ints as long as they exist
    1. insert the pair into the counterset, with the first int as the key and the second as the count


### Functions

Here are the function declarations of those necessary to querier.

```c
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
void rankAndPrint(counters_t* idScores, char* pageDirectory);
void countFunc(void* arg, const int key, const int count);
void sortFunc(void* arg, const int key, const int count);
```

### Usage

The querier module implemented in `querier.c` provides the main method to run the query. As this file is not an implementation of a _.h_ file, compiling the executable and running `./querier [args]` is the only way to use the querier.