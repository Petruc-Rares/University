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
    Node *listGroups = NULL;

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
                        removeSocketFromGroup(listGroups, i);
                        FD_CLR(i, &read_fds);
                        continue;
                    }
                    
                    // client wants to be added to a group
                    if (packetFC.type == 0) {
                        // verificam mai intai daca clientul apartine vreunui grup
                        // daca da, nu il mai adaugam pentru ca poate apartine maxim unuia la un moment de timp
                        // daca nu, il adaugam la grup (daca exista, daca nu cream si grupul)
                        Node *group = findSockInGroups(listGroups, i);
                        if (group) {
                            printf("Clientul se afla si in alt grup deja\n");
                            continue;
                        }
                        printf("Clientul nu se afla in niciun grup\n");

                        Node *group_found = find_group_after_name(listGroups, packetFC.groupName);
                        if (group_found == NULL) {
                            printf("Se creeaza un nou grup\n");
                            Info info;
                            info.crtGroupSize = 1;
                            strcpy(info.group_name, packetFC.groupName);
                            info.sockets[0] = i;
                            for (int it = 1; it < MAX_GROUP_SIZE; it++) {
                                info.sockets[it] = -1;
                            }
                            group_found = createNode(info);
                            addFrontNode(&listGroups, group_found);
                        } else {
                            printf("Grupul deja exista\n");
                            printf("Dimensiune grup: %d\n", group_found->content.crtGroupSize);
                            if (group_found->content.crtGroupSize == MAX_GROUP_SIZE) continue;
                            for (int it = 0; it < MAX_GROUP_SIZE; it++) {
                                if (group_found->content.sockets[it] == -1) {
                                    group_found->content.sockets[it] = i;
                                    break;
                                }
                            }
                            group_found->content.crtGroupSize++;
                            for (int it = 0; it < MAX_GROUP_SIZE; it++) {
                                if (group_found->content.sockets[it] != -1) {
                                    printf("Este clientul cu sock: %d\n", group_found->content.sockets[it]);
                                }
                            }
                        }
                    } else if (packetFC.type == 1) {
                        Node *group_after_sock = findSockInGroups(listGroups, i);
                        if (group_after_sock == NULL) {
                            printf("Clientul cu sock_fd: %d nu face parte din niciun grup\n", i);
                            continue;
                        }
                        Node *group = find_group_after_name(listGroups, packetFC.groupName);
                        for (int it = 0; it < MAX_CLIENTS; it++) {
                            if ((group->content.sockets[it] != -1) && (group->content.sockets[it] != i)) {
                                printf("Mesajul se va trimite catre: %d\n", group->content.sockets[it]);
                                strcpy(packetFS.message, packetFC.message);
                                sendBuffer(group->content.sockets[it], (char *)&packetFS, sizeof(packetFromServer));
                            }
                        }
                    } else if (packetFC.type == 2) {
                        printf("Se deconecteaza: %d\n", i);
                        removeSocketFromGroup(listGroups, i);
                    }



                    //printf("buffer in server: %s\n", buffer);
                }
            }
        }
    }

    close(sockfd);

    return 0;
}
