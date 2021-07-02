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

    accounts accts[MAX_NO_ACCOUNTS];
    int crtNoAccounts = 3;
    int clients[MAX_LEN];
    char var[MAX_LEN];
    int index = 0;
    int pos;

    packetFromClient packetFC;
    packetFromServer packetFS;

    strcpy(accts[0].password, "dragon");
    strcpy(accts[0].username, "dragonule12");
    strcpy(accts[1].password, "conuatas");
    strcpy(accts[1].username, "peseprado");
    strcpy(accts[2].username, "murfatlar");
    strcpy(accts[2].password, "crama#11");

    for (int i = 0; i < crtNoAccounts; i++) {
        printf("username: %s, password: %s\n", accts[i].username, accts[i].password);
    }

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

                } else {
                    // s-au primit date pe unul din socketii de client,
                    // asa ca serverul trebuie sa le receptioneze
                    int no_received = receiveBuffer(i, (char *)&packetFC, sizeof(packetFromClient));
                    if (no_received == 0) {
                        FD_CLR(i, &read_fds);
                        break;
                    }
                    printf("packetFC.type: %d\n", packetFC.type);
                    // authentification
                    if (packetFC.type == 0) {
                        char *username = packetFC.username;
                        char *password = packetFC.password;
                        int it = 0;
                        for (it = 0; it < crtNoAccounts; it++) {
                            if ((strcmp(accts[it].username, username) == 0) &&
                            (strcmp(accts[it].password, password) == 0)) {
                                char *message = "Accept";
                                strcpy(packetFS.message, message);
                                sendBuffer(i, (char*) &packetFS, sizeof(packetFromServer));
                                break;
                            }
                        }
                        // credentials did not match
                        if (it == crtNoAccounts) {
                            char message[MAX_MESSAGE_LENGTH];
                            strcpy(message, "Deny");
                            strcpy(packetFS.message, message);
                            printf("Trimit\n");
                            sendBuffer(i, (char*) &packetFS, sizeof(packetFromServer)); 
                            printf("Am trimis\n");
                        }
                    } else if (packetFC.type == 1) {
                        packetFS.noNumbers = 0;
                        memset(packetFS.primesNumbers, 0, MAX_NO_NUMBERS);

                       // int noPrimes = 0;
                        for (int it = 0; it < packetFC.noNumbers; it++) {
                            int crtNumber = packetFC.numbers[it];
                            if (checkPrime(crtNumber) == 1) {
                                printf("%d e prim\n", packetFC.numbers[it]);
                                packetFS.primesNumbers[packetFS.noNumbers++] = crtNumber;
                            } else {
                                printf("%d NU e prim\n", packetFC.numbers[it]);
                            }

                            if (packetFS.noNumbers == 0) {
                                strcpy(packetFS.message, "Nu mi-ai dat niciun nr prim\n");
                            } else {
                                strcpy(packetFS.message, "Mi-ai dat si numere prim\n");
                            }
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
