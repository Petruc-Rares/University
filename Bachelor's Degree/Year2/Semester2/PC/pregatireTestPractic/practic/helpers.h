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
#define MAX_DESCRIPTION_LENGTH 100
#define MAX_NO_FRIENDS 5
#define BUFLEN 256

struct accounts {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct packetFromServer {
    // 0 - for ok
    // 1 - otherwise
    char code;
    char buffer[BUFLEN];
};

struct packetFromClient {
    char buffer[BUFLEN];
};

struct Info {
    int sockfd;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
   // int friends[MAX_NO_FRIENDS];
    char friendUsernames[MAX_NO_FRIENDS][MAX_USERNAME_LENGTH];
    int crtNoFriends;
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

Node *checkCredentials(Node *list, char *username, char *password) {
    while (list) {
        if ((strcmp(list->content.username, username) == 0)
            && (strcmp(list->content.password, password) == 0)) {
                return list;
            } 

        list = list->next;
    }
    return NULL;
}

Info createInfo(char *username, char *password) {
    Info newInfo;
    newInfo.crtNoFriends = 0;
    memset(newInfo.description, 0, MAX_DESCRIPTION_LENGTH);
    memcpy(newInfo.username, username, MAX_USERNAME_LENGTH);
    memcpy(newInfo.password, password, MAX_PASSWORD_LENGTH);
    // no socket associated yet
    newInfo.sockfd = -1;
    for (int i = 0; i < MAX_NO_FRIENDS; i++) {
        // no friends associated yet
        memset(newInfo.friendUsernames[i], 0, MAX_USERNAME_LENGTH);
    }

    return newInfo;
}

Node *findClientBySocket(Node *listClients, int sockfd) {
    while (listClients) {
        if (listClients->content.sockfd == sockfd) {
            return listClients;
        }
        listClients = listClients->next;
    }
    return NULL;
}

Node *findClientByName(Node *listClients, char *name) {
    while (listClients) {
        if (strcmp(listClients->content.username, name) == 0) {
            return listClients;
        }
        listClients = listClients->next;
    }
    return NULL;   
}

void printList(Node *list) {
    Node *it = list;
    while (it != NULL) {
        printf("URMATORUL CLIENT:\n");
        //printf("%d ", it->content.id);
        printf("crtNoFriends: %d\n", it->content.crtNoFriends);
        printf("description: %s\n", it->content.description);
        printf("username: %s\n", it->content.username);
        printf("password: %s\n", it->content.password);
        printf("sock_fd associated: %d\n", it->content.sockfd);
        
        printf("are prieteni pe: ");
        for (int i = 0; i < MAX_NO_FRIENDS; i++) {;
            if (strlen(it->content.friendUsernames[i]) != 0) {
                printf("%s ", it->content.friendUsernames[i]);
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

        //printf("No of sent bytes: %d\n", bytes_sent);

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

     //   printf("No of received bytes: %d\n", bytes_recvd);

        DIE(bytes_recvd < 0, "ERROR in receiving bytes");

        if (bytes_recvd == 0) {
            break;
        }

        bytes_remaining -= bytes_recvd;
    }

    return bytes_recvd;
}

#endif
