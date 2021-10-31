# Lab 6 - Ethan Chen
# CS50 Fall 2021

### querier

The querier module is the third part of the Tiny Search Engine.

It takes a crawler output directory and an index filename. It loads the index, prompts the user for input, and takes the query input to score. When scoring, the querier looks through the index and finds the word, computing an _orSequence_ or an _andSequence_ whenever necessary to generate a _score_. Ands take precedence over Ors. Ands are the minimums of two counts for matching keys in two countersets, while Ors are the sum of them. Finally, the querier rank orders the document IDs by score and prints them out to stdout.

The `querier.c` file runs the querier, prompting for input and supplying relevant URLs as the output.

The `fuzzquery.c` prints to stdout a random string of valid inputs based on the index file.

### Assumptions

The querier does account for most assumptions within the code, although for proper execution there are many conditions. It assumes
* the right number of arguments are given (2)
* the _pageDir_ exists, and is a valid crawler-filled directory
* the _indexFilename_ file exists, and is of the form of a index output document
* all of the URLs in the index file are normalized, as they technically should be
* there is enough memory on the computer to handle the tasks
* all of the necessary .o files exist for compilation
* the program assumes that the directory and index file are located inside data, e.g. wikipedia-index-0 rather than ../data/wikipedia-index-0. The program automatically appends the filepath prefix

A final limitation to this querier is similar to that from the crawler and indexer, as shown as such in the example of letters-2. Since a page leads back to the home page and is not recognized  as the same page, there will be a single copy of that page. According to EdDiscussion, this is alright, albeit a limitation. However, this means that certain words will by default be double counted in different files that are actually the same file.

### Files

* `Makefile` - compilation procedure
* `querier.c` - the implementation
* `fuzzquery.c` - the given fuzz query test module
* `README.md` - extra info about the module
* `DESIGN.md` - a description of the design specs
* `IMPLEMENTATION.md` - a description of my implementation
* `TESTING.md` - a description of my testing practices
* `testing.sh` - shell testing script
* `testing.out` - result of `make test &> testing.out`
* `tests` - a folder containing some files for `testing.sh`
* `.gitignore` - a gitignore file

### Compilation

To compile, call `make all` or `make querier`