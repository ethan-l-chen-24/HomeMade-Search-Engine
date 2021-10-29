# ETHAN CHEN
# testing.sh - testing file for indexer and indextest module

# VALIDS
# --------

# TEST INPUTS TEST
echo "" | ./querier toscrape-depth-0 toscrape-index-0
echo "" | ./querier toscrape-depth-1 toscrape-index-1

# FUZZQUERY TEST
./querier wikipedia-depth-1 wikipedia-index-1 < ./fuzzquery ../data/wikipedia-index-1 5 0
./querier toscrape-depth-1 toscrape-index-1 < ./fuzzquery ../data/toscrape-index-1 5 0
./querier letters-depth-6 letters-index-6 < ./fuzzquery ../data/letters-index-6 5 0

# EDGE CASES
# ----------

# WRONG NUMBER OF ARGUMENTS
./querier sdjflkjdslk sdfjkldslj sdfjklsdj sdfjlkj sdjfklj ejflkdjs < testQueries.txt

# NONEXISTENT FILES
./querier invalidFile invalidFile2 < testQueries.txt

# VALGRIND TESTS
# --------------
valgrind --leak-check=full --show-leak-kinds=all ./querier toscrape-depth-1 toscrape-index-1 < testQueries.txt
valgrind --leak-check=full --show-leak-kinds=all ./querier toscrape-depth-1 toscrape-index-1 < invalidQueries.txt
valgrind --leak-check=full --show-leak-kinds=all ./querier toscrape-depth-1 toscrape-index-1 < ./fuzzquery ../data/toscrape-depth-1 5 0