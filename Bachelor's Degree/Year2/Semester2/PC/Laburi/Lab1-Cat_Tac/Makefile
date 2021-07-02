CC=gcc
OBJ = cat.o tac.o 

all: cat tac

cat: cat.o
	gcc -o cat cat.o

tac: tac.o
	gcc -o tac tac.o

cat.o:
	gcc -c cat.c

tac.o:
	gcc -c tac.c

clean:
	rm *.o tac cat
