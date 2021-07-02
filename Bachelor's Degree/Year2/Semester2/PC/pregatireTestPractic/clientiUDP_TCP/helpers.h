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
#define MAX_MESSAGE_LENGTH 60
#define MAX_GROUP_LENGTH 20
#define MAX_GROUP_SIZE 5

struct accounts {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct packetFromServer {
    char message[MAX_MESSAGE_LENGTH];
};

struct packetFromClient {
    char group[MAX_GROUP_LENGTH];
};

struct Info {
    int socks[MAX_GROUP_SIZE];
    char group[MAX_GROUP_LENGTH];
    int sizeGroup;
};

struct Node {
    Info content;
    Node *next;
};

Node *findGroupByName(Node *list, char *name) {
    while (list) {
        if (strcmp(list->content.group, name) == 0) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

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

Info createInfo(int sockfd, char *name) {
    Info newInfo;
    newInfo.socks[0] = sockfd;
    strcpy(newInfo.group, name);
    newInfo.sizeGroup = 1;
    for (int i = 1; i < MAX_GROUP_SIZE; i++) {
        newInfo.socks[i] = -1;
    }

    return newInfo;
}

Node *createNode(Info info) {
    Node *newNode = malloc(sizeof(Node));
    printf("sizeGroup: %d", info.sizeGroup);
    //memcpy(&newNode->content, &info, sizeof(Info));
    newNode->content = info;
    newNode->next = NULL;

    return newNode;
}

void addToGroup(Node *group, int newSock) {
    if (group == NULL) return;
    for (int i = 0; i < MAX_GROUP_SIZE; i++) {
        if (group->content.socks[i] == -1) {
            group->content.socks[i] = newSock;
            group->content.sizeGroup++;
            return;
        }
    }
}

void deleteFromGroup(Node *group, int sock) {
    if (group == NULL) return;
    for (int i = 0; i < MAX_GROUP_SIZE; i++) {
        if (group->content.socks[i] == sock) {
            group->content.socks[i] = -1;
            group->content.sizeGroup--;
            return;
        }
    }
}

void printList(Node *list) {
    Node *it = list;
    while (it != NULL) {
      //  printf("%d ", it->content.id);
        printf("grupul cu numele: %s are %d participanti\n", it->content.group, it->content.sizeGroup);
        printf("Acestia sunt: ");
        for (int i = 0; i < MAX_GROUP_SIZE; i++) {
            if (it->content.socks[i] != -1) {
                printf("%d ", it->content.socks[i]);
            }
        }
        printf("\n");
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
        printf("No of remaining bytes: %d\n", bytes_remaining);
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
        printf("No of remaining bytes: %d\n", bytes_remaining);
    }

    return bytes_recvd;
}

#endif
