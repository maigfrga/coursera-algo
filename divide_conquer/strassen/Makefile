CC=gcc
LIBDIR=../../utils/
LIBS=ntutils.o
HEADER_LIB=../../utils/
SRC=strassen.c
OUTPUT=strassen
OBJS=$(SRC:.c=.o)

CFLAGS=-Wall -g

strassen:
	@echo application Makefile - linking
	@$(CC) $(SRC) -o  $(OUTPUT) -I$(HEADER_LIB) $(LIBDIR)$(LIBS)

clean:
	rm -f $(OUTPUT)
