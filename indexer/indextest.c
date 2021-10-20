#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(const int argc, char* argv[]) 
{
    char* program = argv[0];
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [oldIndexFilename] [newIndexFilename]");
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

    
}