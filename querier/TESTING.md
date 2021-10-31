# Lab 6 - Ethan Chen
# CS50 Fall 2021

_Unit testing._

Unit testing was implemented within the `querier.c` file. This can be run by toggling the `-DUNITTEST` flag in the makefile. By toggling the flag, it will replace the regular querier with its unit tests. There was one test for every method I thought necessary to test:

* `countWordsInQuery()`
* `parseQuery()`
* `normalizeQuery()`
* `orSequence()`
* `andSequence()`
* `countFunc()`
* `sortFunc()`

It is worth noting that for `sortFunc()` in particular, I tested the sorted case, reverse-order case, and an average case scenario to show that all three work.

It is worth noting that this coding was not as defensive because it is unit testing, and I believe readability to be more important in this case.

_Integration testing._ 

I assembled the querier and test it as a whole. In each case, I examined the output carefully to be sure the scores and documents are correct. Additionally, I tested edge cases for invalid inputs.

1. Tested the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Tested the querier with a pageDir that does not exist and an indexfile that doesn't exist

3. Tested the indexer with the incorrect number of arguments

4. Tested the indexer on some of the crawler directories and index files from the crawler and indexer modules

6. Tested valgrind on several test cases

For the querier, it is important to note that I compared the output to the output given in the lab description, as well as through my own calculations of scores.