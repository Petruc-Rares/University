#ifndef _HELPERS_H
#define _HELPERS_H 1

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct Info Info;
typedef struct Node Node;
typedef struct packetFromClient packetFromClient;
typedef struct packetFromServer packetFromServer;
typedef struct accounts accounts;

#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 40
#define MAX_MESSAGE_LENGTH 50
#define MAX_NAME_LENGTH 30
#define BUFLEN 256

struct accounts {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct packetFromServer {
    char buffer[BUFLEN];
    char message[MAX_MESSAGE_LENGTH];
    char over;
    int bytes;
};

struct packetFromClient {
    // type of request to server
    // 0 - upload
    // 1 - download
    char type;
    char buffer[BUFLEN];
    char nameFile[MAX_NAME_LENGTH];
    // 1 if download is done, 0 otherwise
    // 1 if upload is done, 0 otherwise
    char over;
    int bytes;
};

struct Info {
    int id;
};

struct Node {
    Info content;
    Node *next;
};

void addFrontNode(Node **list, Node *node) {
    if ((*list)== NULL) {
        (*list) = node;
        return;
    }
    node->next = (*list);
    (*list) = node;

    return;
}

void deleteNode(Node **list, Node *node) {
    Node *pointerList = *list;

    // if list is empty, there is nothing
    // to delete
    if (pointerList == NULL) {
        return;
    }

    Node *prev = NULL;
    Node *it = pointerList;
    while (it != NULL) {
        if (it == node) {
            break;
        }
        prev = it;
        it = it->next;
    }

    // element @node was not found
    if (it == NULL) return;

    // element @node is first
    if (prev == NULL) {
        (*list) = (*list)->next;
        // if it was the single elem
        free(node);
        return;
    }

    // element @node is not first elem
    // or is last
    prev->next = it->next;
    free(node);
    return;

}

Node *createNode(Info info) {
    Node *newNode = malloc(sizeof(Node));
    newNode->content = info;
    newNode->next = NULL;

    return newNode;
}

void printList(Node *list) {
    Node *it = list;
    while (it != NULL) {
        printf("%d ", it->content.id);
        it = it->next;
    }
    printf("\n\n");

    return;
}

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

#define BUFLEN		256	// dimensiunea maxima a calupului de date
#define MAX_CLIENTS	5	// numarul maxim de clienti in asteptare

void sendBuffer(int sockfd, char *buffer, int sizeBuffer) {
    int bytes_sent = 0;
    int bytes_remaining = sizeBuffer;
    
    while (bytes_remaining > 0) {
        bytes_sent = send(sockfd, buffer + bytes_sent, sizeBuffer, 0);

        printf("No of sent bytes: %d\n", bytes_sent);

        DIE(bytes_sent < 0, "ERROR in sending bytes");

        if (bytes_sent == 0) {
            break;
        }

        bytes_remaining -= bytes_sent;
    }
}

int receiveBuffer(int sockfd, char *buffer, int sizeBuffer) {
    //memset(buffer, 0, sizeBuffer);

    int bytes_recvd = 0;
    int bytes_remaining = sizeBuffer;
    
    while (bytes_remaining > 0) {
        bytes_recvd = recv(sockfd, buffer + bytes_recvd, sizeBuffer, 0);

        printf("No of received bytes: %d\n", bytes_recvd);

        DIE(bytes_recvd < 0, "ERROR in receiving bytes");

        if (bytes_recvd == 0) {
            break;
        }

        bytes_remaining -= bytes_recvd;
    }

    return bytes_recvd;
}

#endif
