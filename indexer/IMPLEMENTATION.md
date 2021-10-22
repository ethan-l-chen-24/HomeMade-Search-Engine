# Lab 5 - Ethan Chen
# CS50 Fall 2021

### Implementation

Here are short function descriptions of those that are relevant to the indexer.

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