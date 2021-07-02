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
#include <fcntl.h>

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
    int fd;
    int flag = 0;

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

                } else {
                    // s-au primit date pe unul din socketii de client,
                    // asa ca serverul trebuie sa le receptioneze
                    int no_received = receiveBuffer(i, (char *)&packetFC, sizeof(packetFromClient));
                    if (no_received == 0) {
                        FD_CLR(i, &read_fds);
                        close(i);
                        break;
                    }

                    if (flag == 1) {
                        // if upload or download is over
                        if (packetFC.over == 1) {
                            flag = 0;
                            close(fd);
                            continue;
                        }
                    } else {
                        // if is upload
                        if (packetFC.type == 0) {
                            fd = open(packetFC.nameFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            if (fd == -1) {
                                printf("Can't create file for upload\n");
                                continue;
                            }
                        } else if (packetFC.type == 1) {
                            // if client wants to download
                            fd = open(packetFC.nameFile, O_RDONLY);
                            if (fd == -1) {
                                printf("Can't open file for download\n");
                                strcpy(packetFS.message, "Internal server error, might not have the file");
                                sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                                continue;
                            }
                            printf("S-a deschis cu succes fisierul ce urmeaza a fi downloadat\n");
                        }
                        flag = 1;
                    }

                    //printf("Bytes need to be written by server %d\n", packetFC.bytes);
                    if (packetFC.type == 0) {
                        write(fd, packetFC.buffer, packetFC.bytes);
                    } else if (packetFC.type == 1) {
                        strcpy(packetFS.message, "Download in progress");
                        while(1) {
                            char buf[BUFLEN];
                            int bytes_read = read(fd, buf, BUFLEN);
                            printf("Bytes read from server %d\n", bytes_read);
                            packetFS.bytes = bytes_read;
                            
                            if (bytes_read == 0) {
                                packetFS.over = 1;
                                sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                                break;
                            }

                            memcpy(packetFS.buffer, buf, BUFLEN);
                            packetFS.over = 0;
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));     
                        }
                    }
                }
            }
        }
    }

    close(sockfd);

    return 0;
}
