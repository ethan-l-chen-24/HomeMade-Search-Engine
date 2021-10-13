
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"

bool crawler(char* seedURL, char* pageDir, int depth);
bool pagefetcher(webpage_t* page);
char* pagescanner(webpage_t* page, int* pos);
bool pagesaver(webpage_t* page, int* id, char* pageDir);

void itemcount(void* arg, void* item);

int main(int argc, char* argv[]) 
{
    char* program = argv[0];

    if(argc != 4) {
        fprintf(stderr, "Usage: %s [seedURL] [pageDirectory] [maxDepth]\n", program);
        return 1;
    }

    char* seedURLArg = argv[1];
    char* seedURL = malloc(strlen(seedURLArg) + 1);
    strcpy(seedURL, seedURLArg);

    char* pageDirArg = argv[2];
    char* pageDir = malloc(strlen(pageDirArg) + 1);
    strcpy(pageDir, pageDirArg);

    int maxDepth;
    char ignore;
    if (sscanf(argv[3], "%d%c", &maxDepth, &ignore) != 1) {
        fprintf(stderr, "Error: maxDepth must be an integer\n");
        return 1;
    }

    if(crawler(seedURL, pageDir, maxDepth)) {
        return 0;
    } else {
        return 1;
    }
}

bool crawler(char* seedURL, char* pageDir, int maxDepth) 
{
    if(seedURL != NULL && pageDir != NULL) {
        int idCounter = 1;
        bag_t* toCrawl = bag_new();
        hashtable_t* visitedURLs = hashtable_new(30);

        webpage_t* seedPage = webpage_new(seedURL, 0, NULL);
        bag_insert(toCrawl, seedPage);
        if(!hashtable_insert(visitedURLs, seedURL, seedURL)) {
            return false;
        }

        webpage_t* newPage;
        while((newPage = bag_extract(toCrawl)) != NULL) {
            if(!pagefetcher(newPage)) {
                continue;
            }
            if(!pagesaver(newPage, &idCounter, pageDir)) {
                return false;
            }
           
            int currDepth = webpage_getDepth(newPage);

            if(currDepth < maxDepth) {
                char* nextURL;
                int pos = 0;
                while((nextURL = pagescanner(newPage, &pos)) != NULL) {
                    if(!IsInternalURL(nextURL)) {
                        fprintf(stderr, "URL %s is invalid\n", nextURL);
                        continue;
                    }
                    if(hashtable_insert(visitedURLs, nextURL, nextURL)) {
                        webpage_t* newWebpage = webpage_new(nextURL, currDepth + 1, NULL);
                        bag_insert(toCrawl, newWebpage);
                    }   
                }
            }
            webpage_delete(newPage);
        }
        return true;
    } else {
        return false;
    }
}

bool pagefetcher(webpage_t* page) {
    if(page != NULL) {
        char* URL = webpage_getURL(page);
        if(!webpage_fetch(page)) {
            fprintf(stderr, "URL %s was not reachable\n", URL);
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

char* pagescanner(webpage_t* page, int* pos) {

    if(page != NULL && pos != NULL) {
        char* nextURL;
        while((nextURL = webpage_getNextURL(page, pos)) != NULL) {
            return nextURL;
        }
        return NULL;
    } else {
        return NULL;
    }
}

bool pagesaver(webpage_t* page, int* id, char* pageDir) {
    
    if(page != NULL && id != NULL && pageDir != NULL) {

        int idLen = 0;
        int idCopy = *id;
        while(idCopy != 0) {
            idCopy /= 10;
            idLen++;
        }
        
        char slash[] = "/";
        char* idString = count_malloc(idLen * sizeof(char) + 1); 
        sprintf(idString, "%d", *id);
        int destSize = idLen + strlen(pageDir) + 1;

        char fname[destSize];
        strcpy(fname, pageDir);
        strncat(fname, slash, strlen(slash));
        strncat(fname, idString, strlen(idString));

        printf("%s\n", fname);

        FILE *fp = fopen(fname, "w");
        if(fp != NULL) {
           
            fprintf(fp, "%s\n", webpage_getURL(page));
            fprintf(fp, "%d\n", webpage_getDepth(page));
	        fprintf(fp, "%s", webpage_getHTML(page));
	        fclose(fp); 
            *id += 1;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void itemcount(void* arg, void* item)
{
  int* nitems = arg;

  if (nitems != NULL && item != NULL)
    (*nitems)++;
}