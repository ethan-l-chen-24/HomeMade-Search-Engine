# Makefile for 'common' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# edited for common by Ethan Chen, Oct. 2021

OBJS = pagedir.o word.o index.o
LIBS = $L/libcs50.a $C/common.a
LIB = common.a

# build archive file
$(LIB): $(OBJS) 
	ar cr $(LIB) $(OBJS)

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o