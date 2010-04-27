CC=/cygdrive/c/mingw/bin/gcc
CFLAGS=-Wextra -Wall -Os -Wno-unused

all: tladisasm.dll

tladisasm.dll:	tladisasm.c Makefile
		$(CC) $(CFLAGS) -shared -o $@ $<

install:	tladisasm.dll
		cp $< "/cygdrive/c/Program Files/TLA 700/Supports/tladisasm/tladisasm.dll"

clean:

		rm -f tladisasm.dll
