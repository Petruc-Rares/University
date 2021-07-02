CC = gcc
CFLAGS = -std=c9x
EXEC1 = charTest
EXEC2 = longTest
HEADERFILE = OrderedSet.h

build: $(EXEC1) $(EXEC2)

$(EXEC1): charTest.c $(HEADERFILE)
	$(CC) $(CFLAFS) charTest.c -o $(EXEC1)

$(EXEC2): longTest.c $(HEADERFILE)
	$(CC) $(CFLAGS) longTest.c -o $(EXEC2)

clean: 
	rm -f $(EXEC1) $(EXEC2)
