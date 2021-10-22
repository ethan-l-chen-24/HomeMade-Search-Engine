# Makefile for 'common' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# edited for common by Ethan Chen, Oct. 2021

L = ../libcs50
OBJS = pagedir.o word.o index.o 
LIBS = $L/libcs50.a 
LIB = common.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L
CC = gcc

# build archive file
$(LIB): $(OBJS) $(LIBS)
	ar cr $(LIB) $(LIBS) $(OBJS) 

pagedir.o: pagedir.h word.h $L/memory.h $L/webpage.h $L/file.h 
index.o: index.h pagedir.h word.h $L/hashtable.h $L/counters.h $L/webpage.h  $L/file.h $L/memory.h
word.o: word.h $L/memory.h 

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o