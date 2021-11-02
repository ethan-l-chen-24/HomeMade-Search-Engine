# ETHAN CHEN
# testing.sh - testing file for indexer and indextest module

# CREATE FUZZQUERYFILES

./fuzzquery ../data/wikipedia-index-1 5 0 > tests/fqWiki.txt
./fuzzquery ../data/toscrape-index-1 5 0 > tests/fqToscrape.txt
./fuzzquery ../data/letters-index-6 5 0 > tests/fqLetters.txt

# VALIDS
# --------

# TEST INPUTS TEST

./querier toscrape-depth-0 toscrape-index-0 < tests/testQueries.txt

./querier toscrape-depth-1 toscrape-index-1 < tests/testQueries.txt

./querier letters-depth-1 letters-index-1 < tests/testQueries.txt

./querier letters-depth-6 letters-index-6 < tests/testQueries.txt

./querier wikipedia-depth-1 wikipedia-index-1 < tests/testQueries.txt

# FUZZQUERY TESTS

./querier wikipedia-depth-1 wikipedia-index-1 < tests/fqWiki.txt

./querier toscrape-depth-1 toscrape-index-1 < tests/fqToscrape.txt

./querier letters-depth-6 letters-index-6 < tests/fqLetters.txt


# EDGE CASES
# ----------

# WRONG NUMBER OF ARGUMENTS
./querier sdjflkjdslk sdfjkldslj sdfjklsdj sdfjlkj sdjfklj ejflkdjs < tests/testQueries.txt

# NONEXISTENT FILES
./querier invalidFile invalidFile2 < testQueries.txt

# VALGRIND TESTS
# --------------

valgrind --leak-check=full --show-leak-kinds=all ./querier toscrape-depth-1 toscrape-index-1 < tests/testQueries.txt

valgrind --leak-check=full --show-leak-kinds=all ./querier toscrape-depth-1 toscrape-index-1 < tests/invalidQueries.txt

valgrind --leak-check=full --show-leak-kinds=all ./querier toscrape-depth-1 toscrape-index-1 < tests/fqToscrape.txt