# Makefile for 'set' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# edited for set by Ethan Chen, Oct. 2021
L = ../libcs50
C = ../common

OBJS = crawler.o $L/file.o $L/memory.o  $C/pagedir.o
LIBS = -lm
LLIBS = $L/libcs50-given.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# editable test values
SEED_DOMAIN = http://cs50tse.cs.dartmouth.edu/tse
URL = letters
DEPTH = 2

crawler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $@

crawler.o:  $L/file.h $L/memory.h $L/bag.h $L/hashtable.h

$C/pagedir.o: $C/pagedir.h

.PHONY: all test valgrind clean run

all: crawler

# expects a file `test.names` to exist; it can contain any text.
test: crawler testing.sh
	./testing.sh 

run: crawler
	./crawler $(SEED_DOMAIN)/$(URL)/ $(URL)-depth-$(DEPTH) $(DEPTH)

# expects a file `test.names` to exist; it can contain any text.
valgrind: crawler
	$(VALGRIND) ./crawler $(SEED_DOMAIN)/$(URL)/ valgrind $(DEPTH)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f settest
	rm -f core
