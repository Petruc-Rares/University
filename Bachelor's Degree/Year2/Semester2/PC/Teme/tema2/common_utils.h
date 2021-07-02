#ifndef COMMON_UTILS_STRUCTURES
#define COMMON_UTILS_STRUCTURES

#include <netinet/in.h>
#include <stdlib.h>

#define CONTENT_LEN 1501
#define TOPIC_LEN 51
#define MESSAGE_CLIENT_LEN 1000


/*
 * Macro de verificare a erorilor
 * Exemplu:
 *     int fd = open(file_name, O_RDONLY);
 *     DIE(fd == -1, "open failed");
 */

#define DIE(assertion, call_description)	\
	do {									\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",	\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(EXIT_FAILURE);				\
		}									\
	} while(0)

#define NON_ACK 0
#define ACK 1
#define SUBSCRIPTION_RELATED 2
#define UDP_MESSAGE 3

typedef struct packetServer {
    char topic[TOPIC_LEN]; // bine cunoscuta
    char tipDate;   // bine cunoscute
    char flag;  // 0-NONACK, 1-ACK 2-ABOUT_SUBSCRIBTION 3-UDP_MESSAGE
	char payload[CONTENT_LEN]; // valoare mesaj
    struct sockaddr_in conexionInfo;
} packetServer;

typedef struct packetClient {
	// contine informatii despre comanda data de client
	// poate fi "subscribe + topic_name + ...""
    char payload[MESSAGE_CLIENT_LEN];
} packetClient;

#endif