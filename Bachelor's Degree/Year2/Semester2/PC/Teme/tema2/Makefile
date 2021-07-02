# Protocoale de comunicatii:
# Tema2
# Makefile

CC = gcc

CFLAGS = -Wall -g

LIBS = -lm

# Portul pe care asculta serverul (de completat)
PORT = 29923

# Adresa IP a serverului (de completat)
IP_SERVER = 127.0.0.1

# ID-ul clientului (se va schimba cand se creeaza un subscriber nou)
ID_CLIENT = GIGI

all: server subscriber

# Compileaza server.c
server: server.c server_utils.o server_utils.h
	gcc server.c server_utils.o -o server $(LIBS)

# Compileaza subscriber.c
client: subscriber.c

.PHONY: clean run_server run_subscriber

# Ruleaza serverul
run_server:
	./server ${PORT}

# Ruleaza subscriberul
run_subscriber:
	./subscriber ${ID_CLIENT} ${IP_SERVER} ${PORT}

clean:
	rm -f subscriber server server_utils *.o
