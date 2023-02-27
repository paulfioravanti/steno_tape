# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Directories
SRCDIR = src
LIBDIR = lib
INCDIR = include

# Files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(LIBDIR)/%.o,$(SRC))
LIB = $(LIBDIR)/libsteno_tape.a
PC = steno_tape.pc
PCIN = steno_tape.pc.in
PREFIX = /usr/local

.PHONY: all clean install

all: $(LIB) $(PC)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(LIBDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCDIR)

$(PC): $(PCIN)
	sed "s|@prefix@|$(PREFIX)|g" $< > $@

clean:
	rm -f $(OBJ) $(LIB) $(PC)

install: all
	install -d $(PREFIX)/lib/pkgconfig
	install -m 644 $(PC) $(PREFIX)/lib/pkgconfig/
	install -d $(PREFIX)/lib
	install -m 644 $(LIB) $(PREFIX)/lib/
	install -d $(PREFIX)/include/steno_tape
	install -m 644 $(INCDIR)/*.h $(PREFIX)/include/steno_tape/
