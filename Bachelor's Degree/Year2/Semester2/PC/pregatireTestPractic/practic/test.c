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
    int i, ret;
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

    packetFromClient packetFC;
    packetFromServer packetFS;

    Node *listPeople = NULL;
    char test[MAX_USERNAME_LENGTH];
    memset(test, 0, MAX_USERNAME_LENGTH);
    printf("test: %s are lungimea: %ld\n", test, strlen(test));


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

                    if (strncmp(packetFC.buffer, "CREATE", 6) == 0) {
                        printf("CREATE ENCOUNTERED\n");
                        char username[MAX_USERNAME_LENGTH];
                        char password[MAX_PASSWORD_LENGTH];

                        sscanf(packetFC.buffer, "CREATE %s %s\n", username, password);

                        printf("se doreste a fi creat\n");
                        printf("user: %s, lung_user: %ld, password: %s, lung_password: %ld\n", 
                        username, strlen(username), password, strlen(password));

                        Info newInfo = createInfo(username, password);
                        Node *newClient = createNode(newInfo);
                        addFrontNode(&listPeople, newClient);
                    
                        printList(listPeople);
                    } else if (strncmp(packetFC.buffer, "LOGIN", 5) == 0) {
                        char username[MAX_USERNAME_LENGTH];
                        char password[MAX_PASSWORD_LENGTH];

                        sscanf(packetFC.buffer, "LOGIN %s %s\n", username, password);

                        Node *match = checkCredentials(listPeople, username, password);
                        if ((match == NULL) || (match->content.sockfd != -1)) {
                            packetFS.code = -1;
                        } else {
                            packetFS.code = 0;
                            // update sockfd for crt connection
                            match->content.sockfd = i;
                        }
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        printf("After login\n");
                        printList(listPeople);


                    } else if (strncmp(packetFC.buffer, "UPDATE_DESC", 11) == 0) {

                    } else if (strncmp(packetFC.buffer, "ADD_FRIEND", 10) == 0) {

                    } else if (strncmp(packetFC.buffer, "SHOW_INFO", 9) == 0) {

                    } else if (strncmp(packetFC.buffer, "SHOW_PEOPLE_I_MIGHT_KNOW", 24) == 0) {

                    } else if (strncmp(packetFC.buffer, "LOGOUT", 6) == 0) {

                    }
                }
            }
        }
    }

    close(sockfd);

    return 0;
}
