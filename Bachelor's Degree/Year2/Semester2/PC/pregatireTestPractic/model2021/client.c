#include "helpers.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void usage(char *file) {
    fprintf(stderr, "Usage: %s server_address server_port\n", file);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, ret;
    struct sockaddr_in serv_addr;
    char buffer[BUFLEN];

    if (argc < 3) {
        usage(argv[0]);
    }

    fd_set read_fds;
	fd_set tmp_fds;
    // se goleste multimea de descriptori de citire (read_fds) si cea temporara
    FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    DIE(sockfd < 0, "socket");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    ret = inet_aton(argv[1], &serv_addr.sin_addr);
    DIE(ret == 0, "inet_aton");

    ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    DIE(ret < 0, "connect");

    // adaugam cei doi socketi, cel de la tastatura si cel al serverului
    FD_SET(sockfd, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

   // packetFromClient *packetFC;
    packetFromServer packetFS;

    packetFromClient packetFC;

    while (1) {
		tmp_fds = read_fds;
        ret = select(sockfd + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select");

        if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
            memset(buffer, 0, BUFLEN);
            fgets(buffer, BUFLEN - 1, stdin);
            // remove \n at the end
            buffer[strlen(buffer) - 1] = '\0';

            if (strncmp(buffer, "exit", 4) == 0) {
                break;
            }

            // else it is sending info to server
            if (strlen(buffer) != 2) {
                printf("Invalid input\n");
                continue;
            }
            memcpy(&packetFC, buffer, sizeof(packetFromClient));
            sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
            //packetFC = (packetFromClient *) buffer;
            //printf("row: %d, col: %d\n", packetFC->idxRow, packetFC->idxCol);
            // se trimite mesaj la server
            //sendBuffer(sockfd, (char *)packetFC, sizeof(packetFromClient));
        } else if (FD_ISSET(sockfd, &tmp_fds)) {
            int no_received = receiveBuffer(sockfd, (char *) &packetFS, sizeof(packetFromServer));
            if(no_received == 0) break;

            fprintf(stderr, "Message Received: %s\n", packetFS.message);
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if(packetFS.table[i][j] == 1) {
                        printf("x");
                    } else if (packetFS.table[i][j] == 2) {
                        printf("0");
                    } else if (packetFS.table[i][j] == 0) {
                        printf("_");
                    }
                }
                printf("\n");
            }

            if ((strcmp(packetFS.message, "You Won!") == 0) || (strcmp(packetFS.message, "You Lost!") == 0)
            || (strcmp(packetFS.message, "Tie!") == 0) || (strcmp(packetFS.message, "Disconnect!") == 0)) {
                break;
            }
        }
    }

    close(sockfd);

    return 0;
}