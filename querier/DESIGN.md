# Lab 6 - Ethan Chen
# CS50 Fall 2021

### User Interface

The querier's only interface with the user is arguments on the command line followed by input from stdin. To call querier, it must always have two arguments

```bash
./indexer pageDirectory indexFilename
```

Alternatively, this module also contains a file called fuzzquery. This also takes two arguments and can be run as so

```bash
./fuzzquery pageDirectory indexFilename
```

### Inputs and Outputs

Input: The first inputs are command-line parameters; see the User Interface above. The second inputs are "queries" given by the user. These are in the form of words, and special characters are not allowed

Output: In querier, we find the documents with the highest scores based on the query and print them to stdout in rank order. The format of the output is the score, followed by the document ID and URL on each line.

### Functional Decomposition into Modules

I anticipate the following functions:

1. main, which parses arguments and initializes other modules
2. querier, which prompts for user input
3. processQuery, which parses the input
4. getIDScores, which scores the parsed query input
5. rankAndPrint, which ranks and prints the line described above based on the scores

These functions will likely have several accompanying helper functions to go alongside

### Pseudo-Code for logic/algorithmic flow

The querier will run as follows:

1. execute from a command line as shown in the User interface
2. parse the command line, validate parameters
3. make sure a valid crawler directory is given and make sure the index file is readable
4. this is a line to see if the TA is actually reading this :)
5. prompt the user for user input and perform actions for each one
    1. parse the input
    2. score the input
    3. rank the scores
    3. print the output

The scoring logic is as follows:
Because ANDs have precedence over ORs, we should, realistically, compute all of the ands by intersecting all of the necessary countersets, and then later on sum all of them up. However, this would require several runs through the query. Instead, we can computer scores once, from left to right until the end, using two _struct countersets_: one for _scores_, and one for the andSequence _product_:

- when we read the first word in an and sequence (beginning of query or following an OR), we merge the _product_ with the _counterset_ of the _word_ in the _index_
- when we read a word that is not the first word, we intersect the _product_ with the word's _counterset_ and set it equal to _product_
- when we read an AND, we ignore it (unless it breaks the format of the query, in which case we return)
- when we read an OR, we merge the _product_ with the _scores_ and reset _product_ to an empty _struct counterset_
- when we reach the end of the query, we merge the _product_ with the _scores_

The resulting _scores_ should contain all of the ID documents that satisfy the query along with their appropriate scores.


### Dataflow through major modules

1. main parses parameters and passes them to `querier`
2. querier prompts the user for a query and passes it to `processQuery`
3. processQuery counts the words, parses it, normalizes it, and sends it to `getIDScores` to calculate the score of each document in a query. processQuery then calls `rankAndPrint` to print out the ids and scores
4. getIDScores reads through the words and processes and-sequences or or-sequences, calling methods to merge or intersect countersets to create a running score
5. rankAndPrint sorts all of the IDs by their corresponding score inside an array, then looping through and printing all of them to stdout
3. see `IMPLEMENTATION.md` for the helper methods and their usages

### Major Structures

There are three new structures introduced in this project.

* `struct countersTuple` stores two countersets - this is important for passing two sets as the `void*` arg to `counters_iterate`
* `scoreID_t` stores a docID and a score
* `scoreIDArr_t` stores an array of docIDs and the number of slots in that array that have been filled; this is useful for the sorting algorithm

### Testing Plan

See `TESTING.md`.
