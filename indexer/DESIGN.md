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

And some helper modules that provide data structures:

1. _index_ of words and their associated files and frequencies

Those helper modules also provide very important functions for loading, reading, and writing the _index_ to a file. See `index.h` for more information.

1. 

### Pseudo-Code for logic/algorithmic flow

The indexer will fun as follows:

1. execute from a command line as shown in the User interface
2. parse the command line, validate parameters, initialize other modules
3. 

### Dataflow through modules

1. main parses parameters and passes them to the indexer
2. indexer calls index methods to 

### Major Data Structures

The major data structure of this modle is the _index_, which is really just a specified *hashtable*. The hashtable takes a *char* key which is a word in the index, while its value is a *counters* object that counts the occurrences of that word in each file and has the corresponding file id as the key. The *hashtable* also indirectly uses a *set*.

### Testing Plan

_Unit testing._

_Integration testing._ Assemble the indexer and test it as a whole. In each case, examine the output files carefully to be sure they have the contents of the correct page, with the correct words and correct occurrences of each. 

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Test the indexer with a pageDir that does not exist

3. Test the indexer with a pageDir that exists but isn't a crawler directory

4. Test the indexer on all of the crawler directories made by `make test` in the crawler module

5. Run the same tests on indextest to test the code and also prove that the loadIndex method works well