# Lab 4 - Ethan Chen
# CS50 Fall 2021

### crawler

The *crawler* module is the first part of the Tiny Search Engine.
It "crawls" a website for URLs and extracts those that are within the _cs50tse_ domain.
It then continues to search each of those extracted webpages until a certain _depth_ is reached.
As it searches, it also writes a file to a given _directory_ with the URL, depth, and HTML of each website.
After the crawler completes a cycle, the result should be a directory with one file for each website searched, labeled with a unique _id_ number, counting up from 0.

### Assumptions

### Files

* `Makefile` - compilation procedure
* `crawler.c` - the implementation
* `README.md` - extra info about the module
* `testing.sh` - shell testing script
* `testing.out` - result of `make test &> testing.out`
* `TESTING.md` - a description of the testing

### Compilation

to compile, call `make all` or `make crawler.o`


