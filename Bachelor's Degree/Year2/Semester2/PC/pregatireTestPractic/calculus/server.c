#include "helpers.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LEN 100

void usage(char *file) {
    fprintf(stderr, "Usage: %s server_port\n", file);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    char buffer[BUFLEN];
    struct sockaddr_in serv_addr, cli_addr;
    int n, i, ret;
    socklen_t clilen;

    fd_set read_fds; // multimea de citire folosita in select()
    fd_set tmp_fds;  // multime folosita temporar
    int fdmax;       // valoare maxima fd din multimea read_fds

    if (argc < 2) {
        usage(argv[0]);
    }

    // se goleste multimea de descriptori de citire (read_fds) si multimea temporara (tmp_fds)
    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    DIE(sockfd < 0, "socket");

    portno = atoi(argv[1]);
    DIE(portno == 0, "atoi");

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    DIE(ret < 0, "bind");

    ret = listen(sockfd, MAX_CLIENTS);
    DIE(ret < 0, "listen");

    // se adauga noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
    FD_SET(sockfd, &read_fds);
    fdmax = sockfd;

    int clients[MAX_LEN];
    char var[MAX_LEN];
    int index = 0;
    int pos;

    packetFromClient packetFC;
    packetFromServer packetFS;

    accounts accts[MAX_ACCOUNTS_NO];
    strcpy(accts[0].username, "Ionut");
    strcpy(accts[0].password, "Narcisa210");
    strcpy(accts[1].username, "Cosminel");
    strcpy(accts[1].password, "GeorgicaSpaima");
    strcpy(accts[2].username, "Viorel");
    strcpy(accts[2].password, "Lis24#");
    int crtNoAccounts = 3;
    printf("This is the accounts list that i have now\n");
    for (int it = 0; it < crtNoAccounts; it++) {
        printf("username: %s, password: %s\n", accts[it].username, accts[it].password);
    }

    Node *listClients = NULL;

    while (1) {
        tmp_fds = read_fds;

        ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select");

        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &tmp_fds)) {
                if (i == sockfd) {
                    // a venit o cerere de conexiune pe socketul inactiv (cel cu listen),
                    // pe care serverul o accepta
                    clilen = sizeof(cli_addr);
                    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
                    DIE(newsockfd < 0, "accept");

                    // se adauga noul socket intors de accept() la multimea descriptorilor de citire
                    FD_SET(newsockfd, &read_fds);
                    if (newsockfd > fdmax) {
                        fdmax = newsockfd;
                    }

                    printf("Welcome, client with sock_fd: %d\n", newsockfd);

                    Info info = createInfo(newsockfd);
                    Node *newClient = createNode(info);

                    addFrontNode(&listClients, newClient);
                    printList(listClients);

                } else {
                    // s-au primit date pe unul din socketii de client,
                    // asa ca serverul trebuie sa le receptioneze
                    int no_received = receiveBuffer(i, (char *)&packetFC, sizeof(packetFromClient));
                    if (no_received == 0) {
                        FD_CLR(i, &read_fds);
                        close(i);

                        Node *client = findClientAfterSock(listClients, i);

                        if (client == NULL) {
                            printf("Client shold have been created, something bad happened\n");
                        } else {
                            printf("Deleting client from list of clients");
                            deleteNode(&listClients, client);
                            continue;
                        }
                    }

                    // type calculus
                    if (packetFC.type == 0) {
                        int opd1, opd2;
                        char op;
                        sscanf(packetFC.buffer, "%d %c %d", &opd1, &op, &opd2);
                        printf("opd1 = %d, opd2 = %d, op = %c\n", opd1, opd2, op);

                        int result;
                        if (op == '+') {
                            result = opd1 + opd2;
                        } else if (op == '-') {
                            result = opd1 - opd2;
                        } else if (op == '*') {
                            result = opd1 * opd2;
                        } else if (op == '/') {
                            result = opd1 / opd2;
                        }

                        Node* client = findClientAfterSock(listClients, i);
                        client->content.noCalculus++;

                        printf("Result is: %d\n", result);
                        packetFS.result = result;
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                    } else if (packetFC.type == 1) {
                        printf("Command received from client\n");

                        Node *client = findClientAfterSock(listClients, i);
                        if (client == NULL) {
                            printf("Client should have already been authentificated, smthg bad happened\n");
                        } else {
                            packetFS.result = client->content.noCalculus;
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        }
                        // type command
                    } else if (packetFC.type == 2) {
                        // type authentification
                        printf("Authentification received from client\n");


                        int it;
                        for (it = 0; it < crtNoAccounts; it++) {
                            if ((strcmp(packetFC.username, accts[it].username) == 0) &&
                            (strcmp(packetFC.password, accts[it].password) == 0)) {
                                // 0 - means account was found
                                packetFS.result = 0;
                                sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                                break;
                            }
                        }
                        if (it == crtNoAccounts) {
                            // -1 means account not found
                            packetFS.result = -1;
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        }
                    }

                    //printf("buffer in server: %s\n", buffer);
                }
            }
        }
    }

    close(sockfd);

    return 0;
}
