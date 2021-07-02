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
    int sockTCP, newsockfd, portno;
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

    sockTCP = socket(AF_INET, SOCK_STREAM, 0);
    DIE(sockTCP < 0, "socket");

    portno = atoi(argv[1]);
    DIE(portno == 0, "atoi");

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(sockTCP, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    DIE(ret < 0, "bind");

    ret = listen(sockTCP, MAX_CLIENTS);
    DIE(ret < 0, "listen");

    // pentru socket client UDP
    struct sockaddr_in my_sockaddr, from_station;
    //char buf[BUFLEN];

    /*Deschidere socket*/
    int sockUDP = socket(AF_INET, SOCK_DGRAM, 0);
    DIE(sockUDP < 0, "Socket fail");

    /*Setare struct sockaddr_in pentru a asculta pe portul respectiv */
    from_station.sin_family = AF_INET;
    from_station.sin_port = htons(atoi(argv[1]));
    from_station.sin_addr.s_addr = INADDR_ANY;

    /* Legare proprietati de socket */
    int rs = bind(sockUDP, (struct sockaddr *)&from_station, sizeof(my_sockaddr));
    DIE(rs < 0, "Bind fail");

    // se adauga noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
    FD_SET(sockTCP, &read_fds);
    FD_SET(sockUDP, &read_fds);
    fdmax = sockUDP;

    int clients[MAX_LEN];
    char var[MAX_LEN];
    int index = 0;
    int pos;

    packetFromClient packetFC;
    packetFromServer packetFS;

    socklen_t len = sizeof(struct sockaddr_in);
    Node *listGroups = NULL;
    while (1) {
        tmp_fds = read_fds;

        ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select");

        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &tmp_fds)) {
                if (i == sockTCP) {
                    // a venit o cerere de conexiune pe socketul inactiv (cel cu listen),
                    // pe care serverul o accepta
                    clilen = sizeof(cli_addr);
                    newsockfd = accept(sockTCP, (struct sockaddr *)&cli_addr, &clilen);
                    DIE(newsockfd < 0, "accept");

                    // se adauga noul socket intors de accept() la multimea descriptorilor de citire
                    FD_SET(newsockfd, &read_fds);
                    if (newsockfd > fdmax) {
                        fdmax = newsockfd;
                    }
                    printf("Bine ai venit, utilizatorule cu sockfd: %d\n", newsockfd);

                } else if (i == sockUDP) {
                    printf("S-a primit ceva pe socket-ul de udp\n");
                    int bytes_read = recvfrom(sockUDP, buffer, BUFLEN, 0, (struct sockaddr *)&from_station, &len);
                    if (bytes_read == 0) break;
                    
                    printf("Mesaj primit: %s\n", buffer);
                    Node *groupFound = findGroupByName(listGroups, buffer);
                    if (groupFound == NULL) {
                        printf("N-am cui sa-i trimit. Nu e nimeni interesat de grupul asta\n");
                        continue;
                    }
                    strcpy(packetFS.message, "S-a primit mesaj pe topicul la care esti abonat\n");
                    for (int it = 0; it < MAX_GROUP_SIZE; it++) {
                        if (groupFound->content.socks[it] != -1) {
                            sendBuffer(groupFound->content.socks[it], (char *)&packetFS, sizeof(packetFromServer));
                        }
                    }
                } else {
                    // s-au primit date pe unul din socketii de client,
                    // asa ca serverul trebuie sa le receptioneze
                    printf("Size of packe_from_client: %d\n\n", sizeof(packetFromClient));
                    int no_received = receiveBuffer(i, (char *)&packetFC, sizeof(packetFromClient));
                    Node *groupFound = findGroupByName(listGroups, packetFC.group);
                    if (no_received == 0) {
                        FD_CLR(i, &read_fds);
                        deleteFromGroup(groupFound, i);
                        close(i);
                        break;
                    }

                    printf("Clientul TCP vrea sa se conecteze la grupul cu numele: %s\n", packetFC.group);
                    if (groupFound == NULL) {
                        printf("Grupul nu exista\n");
                        Info newInfo = createInfo(i, packetFC.group);
                        Node *newGroup = createNode(newInfo);

                        addFrontNode(&listGroups, newGroup);
                        printf("Pana in acest moment s-au format urmatoarele grupuri:\n");
                        printList(listGroups);
                    } else {
                        if (groupFound->content.sizeGroup >= MAX_GROUP_SIZE) {
                            printf("Grupul are deja lungime maxima, nu mai acepta membri\n");
                            continue;
                        }
                        // altfel, il adaugam la grupul deja existent
                        addToGroup(groupFound, i);
                        printf("Pana in acest moment s-au format urmatoarele grupuri:\n");
                        printList(listGroups);
                    }
                    //printf("buffer in server: %s\n", buffer);
                }
            }
        }
    }

    close(sockTCP);

    return 0;
}
