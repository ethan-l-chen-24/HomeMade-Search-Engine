# Lab 4 - Ethan Chen
# CS50 Fall 2021

### crawler

The *crawler* module is the first part of the Tiny Search Engine.
It "crawls" a website for URLs and extracts those that are within the _cs50tse_ domain.
It then continues to search each of those extracted webpages until a certain _depth_ is reached.
As it searches, it also writes a file to a given _directory_ with the URL, depth, and HTML of each website.
After the crawler completes a cycle, the result should be a directory with one file for each website searched, labeled with a unique _id_ number, counting up from 0.

### Assumptions

The `crawler.c` should handle most edge cases, but for proper execution, it certainly assumes many things. It assumes
* the right number of arguments are given (3)
* the `seedURL`exists, as does the target directory
* there is enough memory on the computer to handle the tasks
* all of the necessary .o files exist for compilation
* the program assumes that the directory is just the name of the local directory in data, e.g. wikipedia-depth-0 rather than ../data/wikipedia-depth-0. The program automatically appends the filepath prefix

Additionally, because we used a _bag_ to store the webpages, we cannot expect the order of the extracted webpages to be consistently the same (although since our bag is a LinkedList it technically will always be the same). Thus, target output (in theory) may differ based on random factors.

A final limitation to this crawler, as shown as such in the example of letters-depth-2, is that an _index_ page that leads back to the home page does not recognize them as the same page, and thus there will be a single copy of that page. According to EdDiscussion, this is alright, albeit a limitation. 

### Files

* `Makefile` - compilation procedure
* `crawler.c` - the implementation
* `README.md` - extra info about the module
* `testing.sh` - shell testing script
* `testing.out` - result of `make test &> testing.out`
* `TESTING.md` - a description of the testing

### Compilation

To compile, call `make all` or `make crawler.o`

### Testing

See `TESTING.md`


