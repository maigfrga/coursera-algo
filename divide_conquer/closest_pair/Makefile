CC=gcc
LIBDIR=../../utils/
LIBS=ntutils.o
HEADER_LIB=../../utils/
SRC=closest_pair.c

OBJS=$(SRC:.c=.o)

CFLAGS=-Wall -g

merge_sort:
	@echo application Makefile - linking
	@$(CC) $(SRC) -o closest_pair -I$(HEADER_LIB) $(LIBDIR)$(LIBS)

clean:
	rm -f closest_pair
