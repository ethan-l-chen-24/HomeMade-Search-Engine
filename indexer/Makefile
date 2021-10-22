# Makefile for 'indexer' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# edited for indexer by Ethan Chen, Oct. 2021
L = ../libcs50
C = ../common

OBJS = indexer.o indextest.o
LIBS = $L/libcs50.a $C/common.a

# uncomment the following to turn on verbose memory logging
# recomment -DTEST to turn off testing output in stdout
TESTING=-DTEST #-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# editable test values
PAGE_DIRECTORY = letters
DEPTH = 2

.PHONY: all test runindextest valgrind valgrind2 clean run

all: indexer indextest $L/libcs50.a $C/common.a

# expects a file script 'testing.sh' to exist; it can contain any text.
test: indexer testing.sh
	bash -v testing.sh 

runindextest: indextest
	./indextest $(PAGE_DIRECTORY)-index-$(DEPTH) $(PAGE_DIRECTORY)-index-$(DEPTH)-test

run: indexer
	./indexer $(PAGE_DIRECTORY)-depth-$(DEPTH) $(PAGE_DIRECTORY)-index-$(DEPTH)

valgrind: indexer
	$(VALGRIND) ./indexer valgrind valgrind-index

valgrind2: indextest
	$(VALGRIND) ./indexer valgrind-index valgrind-index-test

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f settest
	rm -f core

indexer: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) indexer.o $(LIBS) -o $@

indextest: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) indextest.o $(LIBS) -o $@

