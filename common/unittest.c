/* 
 * unittest.c - unittesting for the index file
 *
 * Ethan Chen, Oct. 2021
 */

#ifdef UNITTEST

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "index.h"
#include "hashtable.h"
#include "counters.h"

    // unit testing for the newIndex function
    int test1() 
    {
        int numFailed = 0;
        index_t* i1 = newIndex(10); // FUNCTION
        if(i1 == NULL) numFailed++;
        hashtable_t* table = getHashtable(i1);
        if(table == NULL) numFailed++;
        deleteIndex(i1);
        return numFailed;
    }

    // unit testing for the buildIndexFromCrawler function
    int test2() 
    {
        int numFailed = 0;
        index_t* i2 = newIndex(800);
        buildIndexFromCrawler("letters-depth-1", i2); // FUNCTION
        hashtable_t* table = getHashtable(i2);
        if(hashtable_find(table, "playground") == NULL) numFailed++;
        if(hashtable_find(table, "page") == NULL) numFailed++;
        if(hashtable_find(table, "home") == NULL) numFailed++;
        if(hashtable_find(table, "the") == NULL) numFailed++;
        if(hashtable_find(table, "this") == NULL) numFailed++;
        if(hashtable_find(table, "for") == NULL) numFailed++;
        if(hashtable_find(table, "tse") == NULL) numFailed++;
        if(hashtable_find(table, "algorithm") == NULL) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "playground"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "page"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "home"), 1) != 2) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "home"), 2) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "algorithm"), 2) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "the"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "this"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "for"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "for"), 2) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "tse"), 1) != 1) numFailed++;
        deleteIndex(i2);
        return numFailed;
    }

    // unit testing for the loadIndexFromFile function
    int test3() 
    {
        int numFailed = 0;
        index_t* i3 = loadIndexFromFile("letters-index-1"); // FUNCTION
        hashtable_t* table = getHashtable(i3);
        if(hashtable_find(table, "playground") == NULL) numFailed++;
        if(hashtable_find(table, "page") == NULL) numFailed++;
        if(hashtable_find(table, "home") == NULL) numFailed++;
        if(hashtable_find(table, "the") == NULL) numFailed++;
        if(hashtable_find(table, "this") == NULL) numFailed++;
        if(hashtable_find(table, "for") == NULL) numFailed++;
        if(hashtable_find(table, "tse") == NULL) numFailed++;
        if(hashtable_find(table, "algorithm") == NULL) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "playground"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "page"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "home"), 1) != 2) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "home"), 2) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "algorithm"), 2) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "the"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "this"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "for"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "for"), 2) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "tse"), 1) != 1) numFailed++;
        deleteIndex(i3);
        return numFailed;
    }

    // unit testing for the indexWebpage function
    int test4() 
    {  
        int numFailed = 0;
        index_t* i4 = newIndex(800);
        char* URL = "http://cs50tse.cs.dartmouth.edu/tse/letters/";
        char* URLCopy = malloc(strlen(URL) + 1);
        strcpy(URLCopy, URL);
        webpage_t* webpage = webpage_new(URLCopy, 1, NULL);
        webpage_fetch(webpage);
        
        int x = 1;
        indexWebpage(i4, webpage, &x); // FUNCTION
        hashtable_t* table = getHashtable(i4);
        if(hashtable_find(table, "playground") == NULL) numFailed++;
        if(hashtable_find(table, "page") == NULL) numFailed++;
        if(hashtable_find(table, "home") == NULL) numFailed++;
        if(hashtable_find(table, "the") == NULL) numFailed++;
        if(hashtable_find(table, "this") == NULL) numFailed++;
        if(hashtable_find(table, "for") == NULL) numFailed++;
        if(hashtable_find(table, "tse") == NULL) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "playground"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "page"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "home"), 1) != 2) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "the"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "this"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "for"), 1) != 1) numFailed++;
        if(counters_get((counters_t*) hashtable_find(table, "tse"), 1) != 1) numFailed++;
        deleteIndex(i4);
        return numFailed;
    }

    // the main method for the unittesting
    int main() 
    {
        int totalFailed = 0;

        // test 1
        int failed = 0;
        printf("Welcome to Unit Testing\n");
        failed += test1();
        if (failed == 0) {
            printf("Test 1 passed!\n");
        } else {
            printf("Test 1 failed!\n");
            totalFailed++;
        }

        // test 2
        failed = 0;
        failed += test2();
        if (failed == 0) {
            printf("Test 2 passed!\n");
        } else {
            printf("Test 2 failed!\n");
            totalFailed++;
        }

        // test 3
        failed = 0;
        failed += test3();
        if (failed == 0) {
            printf("Test 3 passed!\n");
        } else {
            printf("Test 3 failed!\n");
            totalFailed++;
        }

        // test 4
        failed = 0;
        failed += test4();
        if (failed == 0) {
            printf("Test 4 passed!\n");
        } else {
            printf("Test 4 failed!\n");
            totalFailed++;
        }

        if(totalFailed == 0) {
            printf("All tests passed!\n");
            return 0;
        } else {
            printf("ERROR: Test failed\n");
            return 1;
        }
    }
    
#endif