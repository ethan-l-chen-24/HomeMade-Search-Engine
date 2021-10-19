
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/memory.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"


bool indexer(char* pageDir, char* indexFilename);

bool buildIndex(char* pageDir, hashtable_t* index);

static void deleteCT(void *item);


int main(const int argc, char* argv[])
{

    char* program = argv[0];

    if(argc != 3) {
        fprintf(stderr, "Usage: %s [pageDirectory] [indexFilename]\n", program);
        return 1;
    }

    char* pageDirArg = argv[1];
    char* pageDir = count_malloc(strlen(pageDirArg) + 1);
    if (pageDir == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(pageDir, pageDirArg);

    char* indexFnameArg = argv[2];
    char* indexFilename = count_malloc(strlen(indexFnameArg) + 1);
    if (pageDir == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(indexFilename, indexFnameArg);

    if(indexer(pageDir, indexFilename)) {
        return 0;
    } else {
        return 1;
    }
}

bool indexer(char* pageDir, char* indexFilename) 
{
    if(pageDir != NULL && indexFilename != NULL) {
        hashtable_t* index = hashtable_new(800);
        if(!buildIndex(pageDir, index)) {
            fprintf(stderr, "Error: Could not build; invalid directory");
            return false;
        }
        if(!saveIndex(indexFilename, pageDir, index)) {
            return false;
        }
        hashtable_delete(index, deleteCT);
        free(pageDir);
        free(indexFilename);
        return true;
    } else {
        return false;
    }
}

bool buildIndex(char* pageDir, hashtable_t* index) 
{
    if(!validDirectory(pageDir)) {
        return false;
    }
    int id = 1;
    char* idString = intToString(id);
    char* file = stringBuilder(pageDir, idString);
    FILE* fp;
    while ((fp = fopen(file, "r")) != NULL) {
        printf("Reading file %s\n", file);
        indexFile(fp, index, id);
        fclose(fp);
        free(file);
        free(idString);

        id++;
        idString = intToString(id);
        file = stringBuilder(pageDir, idString); // CHANGE
    }
    fclose(fp);
    free(file);
    free(idString);
    return true;
}

/******** delete counters item ********/
static void deleteCT(void *item)
{
	counters_t* ct = item;

	if (ct != NULL) {
		counters_delete(ct);
	}
}