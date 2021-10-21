#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../common/index.h"

bool indexTest(char* newFile, index_t* index);

int main(const int argc, char* argv[]) 
{
    char* program = argv[0];
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [oldIndexFilename] [newIndexFilename]");
        return 1;
    }

    // allocate memory and copy string for pageDir
    char* oldFileArg = argv[1];
    char* oldFile = stringBuilder(NULL, oldFileArg);
    if (oldFile == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 

    // allocate memory and copy string for indexFilename
    char* newFileArg = argv[2];
    char* newFile = stringBuilder(NULL, newFileArg);
    if (newFile == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 

    index_t* index = loadIndex(oldFile);
    if (index != NULL) {
        if (saveIndexToFile(newFile, index)) {
            return 0;
        } else {
            return 1;
        }
    }
}