# Lab 5 - Ethan Chen
# CS50 Fall 2021

### indexer

The _indexer_ module is the second part of the Tiny Search Engine.

It takes a crawler output directory, reads through the words in each file's webpage, and adds them to an index. The index keeps track of a specific word found in the file, along with the number of times it appears and in which files. 

The `indexer.c` file runs the index methods to build the index from the crawler directory, and it then prints an indexer output directory to a given filename

The `indextest.c` takes an index file, loads it into the index struct, and then prints it out to another file. This is a tester for the `loadIndex` function defined in `index.h`.

### Assumptions

The indexer does account for most assumptions within the code, although for proper execution there are many conditions. It assumes
* the right number of arguments are given (2)
* the _pageDir_ exists, and is a valid crawler-filled directory
* there is enough memory on the computer to handle the tasks
* all of the necessary .o files exist for compilation
* the program assumes that the directory is located inside data, e.g. wikipedia-index-0 rather than ../data/wikipedia-index-0. The program automatically appends the filepath prefix

A final limitation to this indexer is similar to that from the crawler, as shown as such in the example of letters-2. Since a page  leads back to the home page and is not recognized  as the same page, there will be a single copy of that page. According to EdDiscussion, this is alright, albeit a limitation. However, this means that certain words will by default be double counted in different files that are actually the same file.

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the implementation
* `README.md` - extra info about the module
* `testing.sh` - shell testing script
* `testing.out` - result of `make test &> testing.out`
* `DESIGN.md` - a description of the design specs
* `IMPLEMENTATION.md` - a description of my implementation
* `.gitignore` - a gitignore file

### Compilation

To compile, call `make all` or `make indexer`