#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "pagedir.h"
#include "word.h"


static void printCT(void* arg, const char* key, void* item);
static void printCTHelper(void* arg, const int key, const int count);
static bool readFile(webpage_t* page, hashtable_t* index, int id);


bool saveIndex(char* filename, char* pageDir, hashtable_t* index)
{
    char* filepath = stringBuilder(pageDir, filename);
    FILE* fp;
	if((fp = fopen(filepath, "w")) != NULL) {
		hashtable_iterate(index, fp, printCT);
        fclose(fp);
	}
    free(filepath);
    return true;
}

bool indexFile(FILE* fp, hashtable_t* index, int id) 
{
    char* URL = freadlinep(fp);
    webpage_t* page = webpage_new(URL, 0, NULL);
    if(!webpage_fetch(page)) {
        fprintf(stderr, "Error: page %s cannot be fetched\n", URL);
        return false;
    }
    readFile(page, index, id);
    webpage_delete(page);
    return true;
}

static bool readFile(webpage_t* page, hashtable_t* index, int id)
{
    int loc = 0;
    char* word;
    while ((word = webpage_getNextWord(page, &loc)) != NULL) {
        normalizeWord(word);
        void* item = hashtable_find(index, word);
        if (item == NULL) {
            counters_t* wordCounter = counters_new();
            counters_add(wordCounter, id);
            hashtable_insert(index, word, wordCounter);
        } else {
            counters_t* wordCounter = (counters_t*) item;
            counters_add(wordCounter, id);
        }
        free(word);
    }
    return true;
}

static void printCT(void* arg, const char* key, void* item) 
{
	FILE* fp = (FILE*) arg;
    if(item != NULL) {
	    fprintf(fp, "%s ", key);
	    counters_t* ctrs = (counters_t*) item;
	    counters_iterate(ctrs, arg, printCTHelper);
    }
	fprintf(fp, "\n");
}

static void printCTHelper(void* arg, const int key, const int count) 
{
    FILE* fp = (FILE*) arg;
	fprintf(fp, "%d %d ", key, count);
}