# Lab 5 - Ethan Chen
# CS50 Fall 2021

### Implementation

The querier is implemented in _C_ using _VSCode_ as an editor. It was also developed using MacOS.

The querier is implemented with respect to the design specs in `DESIGN.md`.

The algorithm follows the pseudocode as described in `DESIGN.md`. Here is the major data flow and pseudocode for all of the modules

#### `main`
Runs the querier

1. validate args
2. call pageDirValidate, and if false, return
3. call querier

#### `query`


#### `processQuery`


#### `countWordsInQuery`


#### `parseQuery`


#### `normalizeQuery`


#### `getIDScores`


#### `orSequence`


#### `andSequence`


#### `countersUnionHelper`


#### `countersIntersectionHelper`


#### `rankAndPrint`


#### `countFunc`


#### `sortFunc`


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