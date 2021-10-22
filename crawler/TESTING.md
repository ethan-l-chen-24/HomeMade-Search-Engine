# Lab 4 - Ethan Chen
# CS50 Fall 2021

### TESTING

Testing was implemented in this lab using a shell script called `testing.sh`. Within the shell script is several calls to the executable, compiled `crawler` file. To call the testing file, simply type `make test`. The testing output has also been printed in `testing.out` using the command `make test &> testing.out`.

The test case begins by deleting and remaking each of out directories in `../data` as to ensure that all of the directories are initially empty. Then, the shell script runs a series of tests.

The first set of tests are tests on the provided TSE websites, as in the webpage `http://cs50tse.cs.dartmouth.edu/tse-output/`. By using all of the same tests (except for toscrape-depth-2 and wikipedia-depth-2, which both take far too long), I was able to compare the number of outputted files and the URLs of those files to my own, which ensured the success of my algorithm.

I then tested several edge cases
* nonexistent directory, should throw an error
* invalid link, should throw an error
* negative depth, should throw an error
* out of CS50 domain, should throw an error
* wrong number of arguments, should, you guessed it, throw an error

As shown by the `testing.out` file, it does do that.

It is worth noting that for the testing.out file, I activated the _macro_ `TEST` in the make file, which enables the printing of saved files and invalid URLs, as well as "success" or "failure" in the case of success of failure within the execution of the crawler itself. That message is not called by errors caught before execution of the algorithm.