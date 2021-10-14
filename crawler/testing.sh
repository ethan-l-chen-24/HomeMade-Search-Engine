
# ETHAN CHEN
# testing.sh - testing file for crawler module

# REMOVE DIRECTORIES IFF EXIST AND MAKE NEW
rm -rf ../common/letters-depth-[0-6]
rm -rf ../common/toscrape-depth-[0-1]
rm -rf ../common/crawler-depth-[0-1]
rm -rf ../common/default
mkdir ../common/default
mkdir ../common/letters-depth-{0..6}
mkdir ../common/toscrape-depth-{0..1}
mkdir ../common/wikipedia-depth-{0..1}

# LETTERS TESTS
# -------------
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters-depth-0 0

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters-depth-1 1

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters-depth-2 2

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters-depth-3 3

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters-depth-4 4

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters-depth-5 5

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters-depth-6 6

# TOSCRAPE TESTS
# --------------
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ toscrape-depth-0 0

./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ toscrape-depth-1 1

# WIKIPEDIA TESTS
# ---------------
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ wikipedia-depth-0 0

./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ wikipedia-depth-0 0

./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ wikipedia-depth-1 1

# NONEXISTENT DIRECTORY TEST
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ non-existent-dir 0

# INVALID LINK
./crawler http://cs50tse.cs.dartmouth.edu/tse/invalidLink/ default 3

# NEGATIVE DEPTH
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ default -1

# OUT OF CS50 DOMAIN
./crawler https://www.google.com/ default 3

# WRONG NUMBER OF ARGUMENTS
./crawler sdfjkldsj sdfjkldjs 23 vdsjlkj
