/* 
 * crawler.c - crawler module for tiny search engine 
 *
 * starting at a "seed" URL, the crawler crawls the link for other links,
 * retrieving and crawling webpages that it finds up to a certain "depth"
 * from the seed. Each URL crawled is saved in its own file with a unique
 * ID number in a given directory name. Each file contains the URL, the 
 * depth, and the webpage's HTML.
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"

/************* function prototypes ********************/

bool crawler(char* seedURL, char* pageDir, int depth);
void processWebpages(hashtable_t* visitedURLs, bag_t* toCrawl, int* idCounter, char* pageDir, int maxDepth);
bool pageFetcher(webpage_t* page);
char* pageScanner(webpage_t* page, int* pos);
bool pageSaver(webpage_t* page, int* id, char* pageDir);

/************* local function prototypes ********************/

static char* stringBuilder(int* id, char* pageDir);
static void delete(void* item);
static void freeStructs(hashtable_t* ht, bag_t* bag);

/************** main() ******************/
/* the "testing" function/main function, which takes three arguments 
 * as inputs (other than the executable call), the URL of the "seed", 
 * the directory in which all of the created files will be stored, 
 * and the maximum depth of the crawl 
 * 
 * Pseudocode:
 *      1. make sure there are exactly 3 other arguments
 *      2. copy the pageDirectory and seedURL into malloc'd strings
 *      3. store the maxDepth as an int
 *      4. call the crawler method
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
 *      2. the directory exists, otherwise throws errors
*/
int main(int argc, char* argv[]) 
{
    char* program = argv[0];
    // check for the appropriate number of arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [seedURL] [pageDirectory] [maxDepth]\n", program);
        return 1;
    }

    // allocate memory and copy string for seedURL
    char* seedURLArg = argv[1];
    char* seedURL = count_malloc(strlen(seedURLArg) + 1);
    if(seedURL == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(seedURL, seedURLArg);

    // NOTE: don't copy string for pageDir since pageDir is constant throughout program
    char* pageDir = argv[2];

    // turn the 3rd input [depth] into an int, if not return error
    int maxDepth;
    char ignore;
    if (sscanf(argv[3], "%d%c", &maxDepth, &ignore) != 1) {
        fprintf(stderr, "Error: maxDepth must be an integer\n");
        return 1;
    }

    // call the crawler function, return successful if so, otherwise
    // free the seedURL and exit unsuccessful 
    if (crawler(seedURL, pageDir, maxDepth)) {
        printf("SUCCESS\n");
        return 0;
    } else {
        if (seedURL != NULL) count_free(seedURL);
        return 1;
    }
}

/************** crawler() ******************/
/* the skeleton code for the crawler, creating necessary variables. 
 * For the actual algorithm code, see processWebpages
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
*/
bool crawler(char* seedURL, char* pageDir, int maxDepth) 
{
    if (seedURL != NULL && pageDir != NULL) {

        // check if the directory is valid by creating a file labeled .crawler
        if (!validDirectory(pageDir)) {
            fprintf(stderr, "Error: The directory %s does not exist\n", pageDir);
            return false;
        }

        // initialize the id counter, bag, and table
        int idCounter = 1;
        bag_t* toCrawl = bag_new();
        hashtable_t* visitedURLs = hashtable_new(30);
        if (toCrawl == NULL || visitedURLs == NULL) {
            // make sure the items are created, handle errors
            fprintf(stderr, "Error: Out of memory\n");
            return false;
        }
        
        // insert into the hashtable, otherwise end the function
        if (!hashtable_insert(visitedURLs, seedURL, "")) {
            freeStructs(visitedURLs, toCrawl);
            count_free(pageDir);
            return false;
        }

        // initialize the seed page and add it to the bag
        webpage_t* seedPage = webpage_new(seedURL, 0, NULL);
        bag_insert(toCrawl, seedPage);

        // run crawl algorithm
        processWebpages(visitedURLs, toCrawl, &idCounter, pageDir, maxDepth);
        
        freeStructs(visitedURLs, toCrawl);
        return true;
    } else {
        // if it fails, free the seedURL
        if (seedURL != NULL) count_free(seedURL);
        return false;
    }
}

/************** processWebpages() ******************/
/* performs the actual "crawl". As long as there are webpages left
 * to search inside the bag, it will go through each, extract URLs, 
 * and create webpages to be added back to the bag. It will also keep
 * track of previously used URLs
 * 
 * Pseudocode:
 *      1. get a webpage from the bag, fetch its HTML, and save its data to a file
 *      2. if still less than the current depth, get all of the URLs embedded in the HTML
 *      3. for each URL, check if it is internal (within cs50tse domain), normalized, and not 
 *              already checked
 *      4. create a new webpage for that URL and insert it into the bag
 *      5. delete each webpage before getting another webpage
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
 *      2. the bag is not empty by default, otherwise nothingn happens
*/
void processWebpages(hashtable_t* visitedURLs, bag_t* toCrawl, int* idCounter, char* pageDir, int maxDepth) 
{
    // go through as long as still webpages in the bag
    webpage_t* newPage;
    while ((newPage = bag_extract(toCrawl)) != NULL) {
        // fetch the HTML of the page
        if (!pageFetcher(newPage)) {
            // if unable to, delete the webpage to free memory and continue to next loop
            webpage_delete(newPage);
            continue;
        }
        // save the page's data to a file in the directory
        if (!pageSaver(newPage, idCounter, pageDir)) {
            // if unable, delete webpage to free memory and continue to next loop
            webpage_delete(newPage);
            continue;
        }
           
        // continue if not already at maxDepth
        int currDepth = webpage_getDepth(newPage);
        if (currDepth < maxDepth) {
             // int to represent the position of the stream in the HTML
             // so that it can pick up where it left off in subsequent loops
            int pos = 0;

            // get all of the URLs embedded in the webpage
            char* nextURL;
            while ((nextURL = pageScanner(newPage, &pos)) != NULL) {
                // check if within cs50tse domain and normalized
                if (!IsInternalURL(nextURL)) {
                    // optional print statement when URL can't be normalized
                    // or is not within cs50tse domain, UNCOMMENT FOR USE
                    //printf("URL %s is invalid!", nextURL);
                    count_free(nextURL);
                    continue;
                }
                // insert the URL into the hashtable
                if (hashtable_insert(visitedURLs, nextURL, "")) {
                    // create a new webpage (without HTML), increment depth, and insert into bag
                    webpage_t* newWebpage = webpage_new(nextURL, currDepth + 1, NULL);
                    bag_insert(toCrawl, newWebpage);
                } else {
                    // if here, the URL already exists, so free it
                    count_free(nextURL);
                }
            }
        }
        webpage_delete(newPage);
    }
}

/************** pageFetcher() ******************/
/* from the URL stored inside a webpage, fetches the content of 
 * that webpage from the web and adds it to that webpage's URL
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
*/
bool pageFetcher(webpage_t* page) 
{
    if (page != NULL) {
        // fetch the HTML from the webpage
        if (!webpage_fetch(page)) {
            char* URL = webpage_getURL(page);
            fprintf(stderr, "Error: URL %s was not reachable\n", URL);
            return false;
        } else {
            return true;
        }
    } else {
        // handle errors
        fprintf(stderr, "Error: the webpage is null");
        return false;
    }
}

/************** pageScanner() ******************/
/* given a webpage, find and return each subsequent URL from the last 
 * found URL (using pointer pos) within the webpage's HTML. If the 
 * scanner has already found all URLs, webpage_getNextURL() returns null
 * 
 * Assumptions:
 *      1. the user puts in valid inputs, otherwise throws errors
*/
char* pageScanner(webpage_t* page, int* pos) 
{
    if (page != NULL && pos != NULL) {
        // get the nextURL and return it
        char* nextURL = webpage_getNextURL(page, pos);
        return nextURL;
    } else {
        return NULL;
    }
}

/************** pageSaver() ******************/
/* takes a webpage and saves it to a file in the given directory
 * the file prints the URL on the first line, then the depth, and then
 * the webpage's HTML.
 * 
 * Pseudocode:
 *      1. check if inputs are valid
 *      2. build the string
 *      3. open the file, if not possible, throw errors
 *      4. print the appropriate lines to the file
 *      5. increment the id number for future pages
 * 
 * Assumptions:
 *      1. inputs are valid, otherwise throw errors  
*/
bool pageSaver(webpage_t* page, int* id, char* pageDir) 
{
    if (page != NULL && id != NULL && pageDir != NULL) {

        // build the string and open the file
        char* fname = stringBuilder(id, pageDir);
        FILE *fp = fopen(fname, "w");
        if (fname != NULL) count_free(fname); // free the memory from the filename 
        if (fp != NULL) {
            // get the URL, depth, and HTML from the webpage
            char* pageURL = webpage_getURL(page);
            int pageDepth = webpage_getDepth(page);
            char* pageHTML = webpage_getHTML(page);

            // print the URL, depth, and HTML to the file
            if(pageURL != NULL) fprintf(fp, "%s\n", pageURL);
            fprintf(fp, "%d\n", pageDepth);
	        if(pageHTML != NULL) fprintf(fp, "%s", pageHTML);
            
	        fclose(fp); // close the file
            *id += 1; // increment the id
            return true;
        } else {
            // handle errors
            fprintf(stderr, "Error: could not create file %s\n", fname);
            return false;
        }
    } else {
        fprintf(stderr, "Error: could not save page %s\n", webpage_getURL(page));
        return false;
    }
}

/************** stringBuilder() ******************/
/* creates the filepath of the file to be created based on the
 * directory name and the id of the current webpage. The path 
 * chosen was in the 'common' directory, which is located parallel
 * to the crawler directory. So, the filepath would be 
 * ../common/[pageDir]/[id]
 * 
 * Pseudocode:
 *      1. make sure the inputs are not null
 *      2. count the number of characters in the id
 *      3. create the rest of the sections of the filepath
 *      4. find the total size of the filepath and malloc
 *      5. print the appropriate filepath to the string and return
 * 
 * Assumptions:
 *      1. the common directory exists and is placed in the loc as described
 *      2. inputs are valid, errors thrown in pageSaver
*/
char* stringBuilder(int* id, char* pageDir) 
{
    if(pageDir != NULL && id != NULL) {

        // count the number of characters/digits in the id
        int idLen = 0;
        int idCopy = *id; // create a copy object to not alter the original id
        while (idCopy != 0) {
            idCopy /= 10;
            idLen++;
        }
        
        // count the number of characters that will be in the final filepath string
        char prefix[] = "../common/";
        char slash[] = "/";
        int destSize = idLen + strlen(pageDir) + strlen(prefix) + strlen(slash) + 1; // +1 for /0 character

        // allocate the space and print the filepath to that memory
        char* filename = count_malloc(destSize);
        sprintf(filename, "../common/%s/%d", pageDir, *id);

        return filename;
    } else {
        return NULL;
    }
}

/************** delete() ******************/
// frees up the item, specifically of bag
void delete(void* item)
{
    // delete the item if it exists
    if (item != NULL) {
        count_free(item);   
    }
}

/************** freeStructs() ******************/
// calls the delete functions on the hashtable and bag structs
 
void freeStructs(hashtable_t* ht, bag_t* bag) 
{
    // call the delete items on each struct
    if (ht != NULL) hashtable_delete(ht, NULL);
    if (bag != NULL) bag_delete(bag, delete);
}