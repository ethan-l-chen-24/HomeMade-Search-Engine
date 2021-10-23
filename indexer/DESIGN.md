# Lab 5 - Ethan Chen
# CS50 Fall 2021

### User Interface

The indexer's only interface with the user is on the command-line; it must always have two arguments

```bash
./indexer pageDirectory indexFilename
```

Alternatively, this module also contains a file called indextest. This also takes two arguments and can be run as so

```bash
./indextest oldIndexFilename newIndexFilename
```

### Inputs and Outputs

Input: The only inputs are command-line parameters; see the User Interface above.

Output: In indexer, we save a file of the given name to the data directory. In indextest, we save a file with the _newIndexFilename_ to the data directory. Within both of those files, we write

* the word on each line, and following each word
* pairs of the ints, where the first is the id of the crawler file the word was found in, and the second is the number of times it shows up in that file

### Functional Decomposition into Modules

We anticipate the following modules or functions:

1. main, which parses arguments and initializes other modules
2. indexer, which calls methods to build the index from the crawler files and then save the created index to an output file

And a helper modulee to provide data structures: _index_ of words and their associated files and frequencies

The _index_ module, as well as a few others also provide very important functions for loading, reading, and writing the _index_ to a file. See `index.h`, `pagedir.h`, and `word.h` for more information. 

### Pseudo-Code for logic/algorithmic flow

The indexer will fun as follows:

1. execute from a command line as shown in the User interface
2. parse the command line, validate parameters, initialize other modules
3. make sure a valid crawler directory is given and make an _index_
4. this is a line to see if the TA is actually reading this :)
5. begin incrementing from 1 and find all files of that name, e.g. data/pageDir/1, data/pageDir/2, etc.
    1. extract the URL and build the webpage
    2. fetch the HTML of that webpage
    3. Read through each word inside that webpage
        1. normalize the word (make it lowercase)
        2. insert it into the index
6. iterate through all of the words in the index
    1. print the word to the output file
    2. print each counter pair to the output file

### Dataflow through modules

1. main parses parameters and passes them to the indexer
2. indexer calls index methods to load from crawler and write to the file
3. see `IMPLEMENTATION.md` for the rest of the index methods and their usages

### Major Data Structures

The major data structure of this modle is the _index_, which is really just a specified *hashtable*. The hashtable takes a *char* key which is a word in the index, while its value is a *counters* object that counts the occurrences of that word in each file and has the corresponding file id as the key. The *hashtable* also indirectly uses a *set*.

### Testing Plan

_Unit testing._

TODOTODOTODO

_Integration testing._ Assemble the indexer and test it as a whole. In each case, examine the output files carefully to be sure they have the contents of the correct page, with the correct words and correct occurrences of each. 

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Test the indexer with a pageDir that does not exist

3. Test the indexer with a pageDir that exists but isn't a crawler directory

4. Test the indexer with the incorrect number of arguments

5. Test the indexer on all of the crawler directories made by `make test` in the crawler module

6. Test valgrind using `make valgrind`

7. Run the same tests on indextest to test the code and also prove that the loadIndex method works well

For the indextest in particular, it is important to note that I compared the test files to the original indexes by eye, as well as to the output given on the tse website. This is how I validated the success of my algorithm.