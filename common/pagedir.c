#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "../libcs50/memory.h"

bool validDirectory(char* directoryName) 
{
    char begin[] = "../common";
    char end[] = ".crawler";
    int newSize = sizeof(directoryName) + sizeof(begin) + sizeof(end) + 1;
    char* testFile = count_malloc(newSize);
    sprintf(testFile, "%s/%s/%s", begin, directoryName, end);

    FILE *fp = fopen(testFile, "w");
    if(testFile != NULL) free(testFile);
    if(fp != NULL) {
        fprintf(fp, "This file is created by 'pagedir' to check if this directory exists");
        fclose(fp);
        return true;
    } else {
        fprintf(stderr, "Error: Invalid directory");
        return false;
    }
}