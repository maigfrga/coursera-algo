CC=gcc
LIBDIR=../../utils/
LIBS=ntutils.o
HEADER_LIB=../../utils/
SRC=linked_list.c

OBJS=$(SRC:.c=.o)

CFLAGS=-Wall -g

linked_list:
	@echo application Makefile - linking
	@$(CC) $(SRC) -o  linked_list -I$(HEADER_LIB) $(LIBDIR)$(LIBS)

	@$(CC) double_linked_list.c -o  double_linked_list -I$(HEADER_LIB) $(LIBDIR)$(LIBS)

clean:
	rm -f linked_list
	rm -f double_linked_list
