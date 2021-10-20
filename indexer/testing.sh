
# ETHAN CHEN
# testing.sh - testing file for index module

# LETTERS TEST
# -------------
./indexer letters-depth-0 letters-index-0

./indexer letters-depth-1 letters-index-1

./indexer letters-depth-2 letters-index-2

./indexer letters-depth-3 letters-index-3

./indexer letters-depth-4 letters-index-4

./indexer letters-depth-5 letters-index-5

./indexer letters-depth-6 letters-index-6

# TOSCRAPE TEST
# -------------
./indexer toscrape-depth-0 toscrape-index-0

./indexer toscrape-depth-1 toscrape-index-1

# WIKIPEDIA TESTS
# ---------------
./indexer wikipedia-depth-0 wikipedia-index-0

./indexer wikipedia-depth-1 wikipedia-index-1

# NONEXISTENT DIRECTORY TEST
./indexer non-existent-dir filename

# WRONG NUMBER OF ARGUMENTS
./indexer sdfjkldsj sdfjkldjs 23 vdsjlkj thislabiskillingme