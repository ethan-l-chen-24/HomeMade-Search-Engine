# Makefile for 'querier' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# edited for querier by Ethan Chen, Oct. 2021
L = ../libcs50
C = ../common

OBJS = querier.o 
LIBS = $C/common.a $L/libcs50.a 

# uncomment the following to turn on verbose memory logging
# recomment -DTEST to turn off testing output in stdout
TESTING=#-DDEBUG

CFLAGS = -Wall -pedantic -std=gnu11 -ggdb $(TESTING) -I$L -I$C 
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# editable test values
PAGE_DIRECTORY = toscrape
DEPTH = 1

.PHONY: all test valgrind clean run

all: querier

# expects a file `test.names` to exist; it can contain any text.
test: querier testing.sh
	bash -v testing.sh 

run: querier
	./querier $(PAGE_DIRECTORY)-depth-$(DEPTH) $(PAGE_DIRECTORY)-index-$(DEPTH)

# expects a file `test.names` to exist; it can contain any text.
valgrind: querier
	$(VALGRIND) ./querier valgrind valgrind-index

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f settest
	rm -f core
	rm -f querier

querier: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@ 