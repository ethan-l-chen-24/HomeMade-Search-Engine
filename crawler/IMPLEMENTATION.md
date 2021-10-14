# Lab 4 - Ethan Chen
# CS50 Fall 2021

### crawler

Refer to `README.md` for specifics on the specs.

### Implementation

The crawler is implemented in `C` USING `VSCode` as an editor. It was also developed using MacOS.

The crawler is implemented according to the pseudocode given in the lab description. The only change is that the validity of the directory is checked at the beginning

The data structures used in this implementation were a `struct bag` and `struct hashtable` as defined in `bag.h` and `hashtable.h`, respectively. The `struct bag`, named _toCrawl_, held a list of all of the webpages that still needed to be visited. The `struct hashtable` held all of the websites' _URLs_ as keys with arbitrary values, so that no _URL_ is visited more than once.

The algorithm works as so: 
* Parse the command line and validate the parameters
* Validate the directory
* make a webpage for the _seedURL_, marked with depth = 0
* add that page to the `bag` of webpages to crawl
* add that _URL_ to the `hashtable` of URLs seen
* loop through the `bag` until empty, each time extracting a webpage
* fetch the _HTML_ of that page and then save it to the file path branching off of the given `directory`
* if the `depth` of that webpage is less than the maximum, scan through the webpage's _HTML_ for all links
* extract those links, check that they are not normalized or internal, and insert them into the `hashtable`
* make a webpage for that _URL_ at `depth` + 1 and add it to the `bag`

This implementation, as mentioned in the lab description, decomposes the objective into a `main` method and 5 submethods to achieve the task.

* main - parses arguments and initializes other modules
* crawler - creates other necessary variables or structs, scans for initial errors
* processWebpages - loops over pages to explore until the list is exhausted
* pageFetcher - fetches a page from a _URL_
* pageScanner - extracts _URLs_ from a page
* pageSaver - outputs a page to the appropriate file

For more specific pseudocode on each method, refer to the comments above each method in the `crawler.c` file. 

Just a quick note on the page saver: the files were saved to a directory inside of the common folder. This is regardless of the placement of the .c file, as long as it is within a directory parallel to the comon directory.

### Usage

The *crawler* module implemented in `crawler.c` provides methods that can be used for the TSE in other modules.
This includes the complete `crawler()` method as well as each submethod used in the process

```c
bool crawler(char* seedURL, char* pageDir, int depth);
void processWebpages(hashtable_t* visitedURLs, bag_t* toCrawl, int* idCounter, char* pageDir, int maxDepth);
bool pageFetcher(webpage_t* page);
char* pageScanner(webpage_t* page, int* pos);
bool pageSaver(webpage_t* page, int* id, char* pageDir);
```