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
int sockfd, n, ret;
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

    packetFromServer packetFS;
    packetFromClient packetFC;

    //  type authentification
    packetFC.type = 2;
    while(1) {
        printf("Enter your credentials:\n");
        char username[MAX_USERNAME_LENGTH];
        printf("Username:");
        memset(username, 0, MAX_USERNAME_LENGTH);
        fgets(username, MAX_USERNAME_LENGTH - 1, stdin);
        username[strlen(username) - 1] = '\0';

        char password[MAX_PASSWORD_LENGTH];
        printf("Password:");
        memset(password, 0, MAX_PASSWORD_LENGTH);
        fgets(password, MAX_PASSWORD_LENGTH - 1, stdin);
        password[strlen(password) - 1] = '\0';

        memcpy(packetFC.username, username, MAX_USERNAME_LENGTH);
        memcpy(packetFC.password, password, MAX_PASSWORD_LENGTH);
        
        sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
        
        receiveBuffer(sockfd, (char *)&packetFS, sizeof(packetFromServer));
        if (packetFS.result == 0) {
            printf("Authentification succeeded\n");
            break;
        } else {
            printf("Authentification failed\n");
        }
    }

    while (1) {
		tmp_fds = read_fds;
        printf("What do you want to do?\n");
        printf("Press 0 for calculus\n");
        printf("Press 1 for command\n");
        ret = select(sockfd + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select");

        if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
            memset(buffer, 0, BUFLEN);
            fgets(buffer, BUFLEN - 1, stdin);

            //printf("buffer client: %s\n", buffer);
            int choiceClient = atoi(buffer);
            if (choiceClient == 0) {
                // will read data for calculus
                printf("Enter data in the following formand:\nopd1 op opd2\n");
                memset(buffer, 0, BUFLEN);
                fgets(buffer, BUFLEN - 1, stdin);
                buffer[strlen(buffer) - 1] = '\0';

                packetFC.type = 0;
                memcpy(packetFC.buffer, buffer, BUFLEN);

                sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
            } else if (choiceClient == 1) {
                // wil give command to server
                printf("Enter the command you want to give to the server\n");
                memset(buffer, 0, BUFLEN);
                fgets(buffer, BUFLEN - 1, stdin);
                buffer[strlen(buffer) - 1] = '\0';

                if (strncmp(buffer, "exit", 4) == 0) {
                    break;
                }

                packetFC.type = 1;
                memcpy(packetFC.buffer, buffer, BUFLEN);

                sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
            }

            // se trimite mesaj la server
            //sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient))
        } else if (FD_ISSET(sockfd, &tmp_fds)) {
            int no_received = receiveBuffer(sockfd, (char *)&packetFS, sizeof(packetFromServer));
            if (no_received == 0) {

                break;
            }

            printf("Result received from server: %d\n", packetFS.result);

//            fprintf(stderr, "Received: %s", buffer);
        }
    }

    close(sockfd);

    return 0;
}