CC=gcc
CFLAGS=-c -Wall
DEPS=$(wildcard *.h)


all: build

build: movies

movies: movies.o commonFunctions.o
	$(CC) movies.o commonFunctions.o -o movies

commonFunctions.o: commonFunctions.c ${DEPS}
	${CC} ${CFLAGS} commonFunctions.c

movies.o: movies.c ${DEPS}
	$(CC) $(CFLAGS) movies.c

run: build
	valgrind --leak-check=full ./movies ${argument1} ${argument2} ${argument3}

clean:
	rm -f movies.o commonFunctions.o movies
	
