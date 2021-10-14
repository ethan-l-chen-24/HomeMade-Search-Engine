# Makefile for 'common' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# edited for common by Ethan Chen, Oct. 2021

OBJS = pagedir.o
LIBS = -lm

LIB = common.a

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h

.PHONY: clean

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
