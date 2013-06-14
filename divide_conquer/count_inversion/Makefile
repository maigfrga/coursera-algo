CC=gcc
LIBDIR=../../utils/
LIBS=ntutils.o
HEADER_LIB=../../utils/
SRC=count_inversion.c
OUTPUT=count_inversion
OBJS=$(SRC:.c=.o)

CFLAGS=-Wall -g

count_inversion:
	@echo application Makefile - linking
	@$(CC)   $(SRC)  -o  $(OUTPUT)  -I$(HEADER_LIB) $(LIBDIR)$(LIBS) 

clean:
	rm -f $(OUTPUT)
