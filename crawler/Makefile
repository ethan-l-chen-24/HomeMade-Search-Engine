# Makefile for 'crawler' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# edited for crawler by Ethan Chen, Oct. 2021
L = ../libcs50
C = ../common

OBJS = crawler.o 
LIBS = $C/common.a $L/libcs50.a 

# uncomment the following to turn on verbose memory logging
# recomment -DTEST to turn off testing output in stdout
TESTING=-DTEST #-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# editable test values
SEED_DOMAIN = http://cs50tse.cs.dartmouth.edu/tse
URL = letters
DEPTH = 2

.PHONY: all test valgrind clean run

all: crawler

# expects a file `test.names` to exist; it can contain any text.
test: crawler testing.sh
	bash -v testing.sh 

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

crawler: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

