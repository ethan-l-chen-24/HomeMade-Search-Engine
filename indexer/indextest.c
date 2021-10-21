#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../libcs50/memory.h"


bool indexTest(char* newFile, index_t* index);

int main(const int argc, char* argv[]) 
{
    char* program = argv[0];
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [oldIndexFilename] [newIndexFilename]", program);
        return 1;
    }
  
    // allocate memory and copy string for pageDir
    char* oldFileArg = argv[1];
    char* oldFile = count_malloc(strlen(oldFileArg) + 1);
    if (oldFile == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(oldFile, oldFileArg);

    // allocate memory and copy string for indexFilename
    char* newFileArg = argv[2];
    char* newFile = count_malloc(strlen(newFileArg) + 1);
    if (newFile == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(newFile, newFileArg);

    index_t* index = loadIndex(oldFile);
    printf("Index loaded\n");
    if (index != NULL) {
        if (saveIndexToFile(newFile, index)) {
            printf("Index saved\n");
            deleteIndex(index);
            count_free(newFile);
            count_free(oldFile);
            return 0;
        } else {
            deleteIndex(index);
            count_free(newFile);
            count_free(oldFile);    
            printf("Something didn't work\n");
            return 1;
        }
    } else {
        count_free(newFile);
        count_free(oldFile);
        printf("test failed");
        return 1;
    }
}