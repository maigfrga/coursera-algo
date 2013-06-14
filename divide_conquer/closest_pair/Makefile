CC=gcc
LIBDIR=../../utils/
LIBS=ntutils.o
HEADER_LIB=../../utils/
SRC=closest_pair.c

OBJS=$(SRC:.c=.o)

CFLAGS=-Wall -g

closest_pair:
	@echo application Makefile - linking
	@$(CC) $(SRC) -lm -o closest_pair -I$(HEADER_LIB) $(LIBDIR)$(LIBS)

clean:
	rm -f closest_pair
