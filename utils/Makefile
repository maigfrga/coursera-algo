CC=gcc
LIB=ntutils
LIBDEST=~/.lib/
CFLAGS=-Wall -c
LIBSRC=ntutils.c
HEADER_LIB=./
LIBOBJ=$(LIBSRC:.c=.o)

install:
	@echo lib Makefile - installing $(LIB)
	@mkdir -p $(LIBDEST)
	@install -m 444 $(LIB) $(LIBDEST)

build_lib:
	@echo lib Makefile - archiving $(LIB)
	@$(AR) r $(LIB) $(LIBOBJ)

compile:
	@echo lib Makefile - compiling $<
	@$(CC) $(CFLAGS) -I$(HEADER_LIB) $(LIBSRC) -o $(LIBOBJ)

#$(LIB): compile  build_lib  install

clean:
	rm -f ntutils.o
	rm -f ntutils.a
	rm -f ntutils
	rm $(LIBDEST)ntutils
