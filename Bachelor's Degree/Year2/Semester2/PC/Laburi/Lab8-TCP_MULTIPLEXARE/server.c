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

                    // trimite lista cu clinetii existenti la noul client
                    clients[index] = newsockfd;
                    index++;

                    if (index > 1) {
                        char users[MAX_LEN] = "Mai exista clientii cu socketii: ";
                        for (int j = 0; j < index - 1; j++) {
                            sprintf(var, "%d", clients[j]);
                    		strcat(users, var);
                    		strcat(users, " ");
                        }
                        strcat(users, "\n");
                        send(newsockfd, users, strlen(users) + 1, 0);
                    }

                    for (int j = 3; j <= fdmax; j++) {
                        if (j != newsockfd && j != sockfd && FD_ISSET(j, &read_fds)) {
                            char msg[MAX_LEN];
                            sprintf(msg, "S-a conectat la server un nou client cu socketul %d\n", newsockfd);
                            n = send(j, msg, strlen(msg) + 1, 0);
                            DIE(n < 0, "send");
                        }
                    }

                    printf("Noua conexiune de la %s, port %d, socket client %d\n",
                           inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), newsockfd);
                } else {
                    // s-au primit date pe unul din socketii de client,
                    // asa ca serverul trebuie sa le receptioneze
                    memset(buffer, 0, BUFLEN);
                    n = recv(i, buffer, sizeof(buffer), 0);
                    DIE(n < 0, "recv");

                    if (n == 0) {
                        // conexiunea s-a inchis
                        printf("Socket-ul client %d a inchis conexiunea\n", i);
                        close(i);

                        // stergere din lista de socketi disponibili
                        for (int c = 0; c < index; c++) {
                            if (clients[c] == i) {
                                pos = c;
                                break;
                            }
                        }
                        for (int c = pos; c < index - 1; c++) {
                            clients[c] = clients[c + 1];
                        }
                        index--;

                        // se scoate din multimea de citire socketul inchis
                        FD_CLR(i, &read_fds);

                        for (int j = 3; j <= fdmax; j++) {
                            if (j != i && j != sockfd && FD_ISSET(j, &read_fds)) {
                                char msg[MAX_LEN];
                                sprintf(msg, "S-a deconectat de la server clientul de pe socketul %d\n", i);
                                n = send(j, msg, strlen(msg) + 1, 0);
                                DIE(n < 0, "send");
                            }
                        }
                    } else {
                        printf("S-a primit de la clientul de pe socketul %d mesajul: %s\n", i, buffer);
                        int j;
                        sscanf(buffer, "%d", &j);
                        n = send(j, buffer, n, 0);
                        DIE(n < 0, "send");
                        printf("S-a trimis catre clientul de pe socketul %d mesajul: %s\n", j, buffer);
                    }
                }
            }
        }
    }

    close(sockfd);

    return 0;
}
