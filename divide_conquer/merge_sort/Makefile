CC=gcc
LIBDIR=../../utils/
LIBS=ntutils.o
HEADER_LIB=../../utils/
SRC=merge_sort.c

OBJS=$(SRC:.c=.o)

CFLAGS=-Wall -g

merge_sort:
	@echo application Makefile - linking
	@$(CC) $(SRC) -o  merge_sort -I$(HEADER_LIB) $(LIBDIR)$(LIBS)

clean:
	rm -f merge_sort
