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
#define MAX_GROUP_NAME_LENGTH 40
#define MAX_GROUP_SIZE 5

struct accounts {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct packetFromServer {
    char message[MAX_MESSAGE_LENGTH];
};

struct packetFromClient {
    // 0 - for authentification
    // 1 - for sending message
    char type;
    char name[MAX_NAME_LENGTH];
    char groupName[MAX_GROUP_NAME_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
};

struct Info {
    int sockets[MAX_GROUP_SIZE];
    int crtGroupSize;
    char group_name[MAX_GROUP_NAME_LENGTH];
};

struct Node {
    Info content;
    Node *next;
};


Node *find_group_after_name(Node *list, char *name_group) {
    while (list) {
        if (strcmp(list->content.group_name, name_group) == 0) return list;
        list = list->next;
    }
    return NULL;
}

Node *findSockInGroups(Node *list, int sock_fd) {
    while (list) {
        Info infoGroup = list->content;
        for (int i = 0; i < MAX_GROUP_SIZE; i++) {
            if (infoGroup.sockets[i] == sock_fd) {
                return list;
            }
        }
        list = list->next;
    }
    return NULL;
}

void removeSocketFromGroup(Node* list, int sock_fd) {
    Info info;
    int flag = 0;
    for (; list != NULL; list = list->next) {
        for (int it = 0;  it < MAX_GROUP_SIZE; it++) {
            if (list->content.sockets[it] == sock_fd) {
                list->content.sockets[it] = -1;
                list->content.crtGroupSize--;
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0) {
        printf("Nu am gasit socket-ul\n");
        return;
    }

    return ;
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

Node *createNode(Info info) {
    Node *newNode = malloc(sizeof(Node));
    newNode->content = info;
    newNode->next = NULL;

    return newNode;
}

void printList(Node *list) {
    Node *it = list;
    while (it != NULL) {
       // printf("%d ", it->content.id);
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
