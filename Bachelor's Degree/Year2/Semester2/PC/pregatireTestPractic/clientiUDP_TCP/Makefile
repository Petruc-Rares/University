# Protocoale de comunicatii:
# Pregatire Test Practic
# Makefile

CFLAGS = -Wall -g

# Portul pe care asculta serverul (de completat)
PORT = 60017

# Adresa IP a serverului (de completat)
IP_SERVER = 127.0.0.1

all: server client clientUDP.c

# Compileaza server.c
server: server.c

# Compileaza client.c
client: client.c

clientUDP: clientUDP.c

.PHONY: clean run_server run_client run_client_UDP

# Ruleaza serverul
run_server:
	./server ${PORT}

# Ruleaza clientul
run_client:
	./client ${IP_SERVER} ${PORT}

run_client_UDP:
	./clientUDP ${IP_SERVER} ${PORT}

clean:
	rm -f server client clientUDP
