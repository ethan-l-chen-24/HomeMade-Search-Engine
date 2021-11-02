# ETHAN CHEN
# testing.sh - testing file for indexer and indextest module

# CREATE FUZZQUERYFILES

./fuzzquery ../data/wikipedia-index-1 5 0 > tests/fqWiki.txt
./fuzzquery ../data/toscrape-index-1 5 0 > tests/fqToscrape.txt
./fuzzquery ../data/letters-index-6 5 0 > tests/fqLetters.txt

# VALIDS
# --------

# TEST INPUTS TEST

./querier ../data/toscrape-depth-0 ../data/toscrape-index-0 < tests/testQueries.txt

./querier ../data/toscrape-depth-1 ../data/toscrape-index-1 < tests/testQueries.txt

./querier ../data/letters-depth-1 ../data/letters-index-1 < tests/testQueries.txt

./querier ../data/letters-depth-6 ../data/letters-index-6 < tests/testQueries.txt

./querier ../data/wikipedia-depth-1 ../data/wikipedia-index-1 < tests/testQueries.txt

# FUZZQUERY TESTS

./querier ../data/wikipedia-depth-1 ../data/wikipedia-index-1 < tests/fqWiki.txt

./querier ../data/toscrape-depth-1 ../data/toscrape-index-1 < tests/fqToscrape.txt

./querier ../data/letters-depth-6 ../data/letters-index-6 < tests/fqLetters.txt


# EDGE CASES
# ----------

# WRONG NUMBER OF ARGUMENTS
./querier sdjflkjdslk sdfjkldslj sdfjklsdj sdfjlkj sdjfklj ejflkdjs < tests/testQueries.txt

# NONEXISTENT DIRECTORY
./querier invalidDirectory ../data/toscrape-index-0 < tests/testQueries.txt

# NONEXISTENT TESTING FILE
./querier ../data/toscrape-depth-0 invalidFile < tests/testQueries.txt

# VALGRIND TESTS
# --------------

valgrind --leak-check=full --show-leak-kinds=all ./querier ../data/toscrape-depth-1 ../data/toscrape-index-1 < tests/testQueries.txt

valgrind --leak-check=full --show-leak-kinds=all ./querier ../data/toscrape-depth-1 ../data/toscrape-index-1 < tests/invalidQueries.txt

valgrind --leak-check=full --show-leak-kinds=all ./querier ../data/toscrape-depth-1 ../data/toscrape-index-1 < tests/fqToscrape.txt