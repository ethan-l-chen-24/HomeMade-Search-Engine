# Lab 5 - Ethan Chen
# CS50 Fall 2021

### Implementation

The indexer is implemented in _C_ using _VSCode_ as an editor. It was also developed using MacOS.

The indexer is implemented with respect to the design specs in `DESIGN.md`.

The major data structure, as mentioned, is a `struct index` as defined in `index.h`. It is a wrapper struct for a `struct hashtable` as defined in `hashtable.h`, but its methods initialize it more specifically as a `struct hashtable` with a `char*` as its key (as usual) and a `struct counter` as its value. The `struct counters` is defined in `counters.h`

### Functions

Here are the function declarations of those necessary to indexer.

#### index.h
```c
index_t* newIndex(const int tableSize);
void deleteIndex(index_t* index);
bool saveIndexToFile(char* filename, index_t* index);
bool buildIndexFromCrawler(char* pageDir, index_t* index);
index_t* loadIndex(char* filepath);
bool indexWebpage(index_t* index, webpage_t* webpage, int id);
```

#### pagedir.h
```c
bool pageDirValidate(char* pageDir);
webpage_t* loadPageToWebpage(char* pageDir, int* id);
char* stringBuilder(char* pageDir, char* end);
```

#### word.h
```c
void normalizeWord(char* word);
char* intToString(int x);
```

### Usage

The _indexer_ module implemented in `indexer.c` provides the main `indexer()` method that can be used in other modules. However, the majority of the methods created for the indexer are in the _common_ directory, as shown above. These can be called from any other module as long as the makefile imports the `common.a` archive correctly.