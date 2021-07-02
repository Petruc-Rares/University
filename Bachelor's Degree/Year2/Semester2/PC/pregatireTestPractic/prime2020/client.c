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

    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    // send credentials
    while(1) {

        printf("Enter your username: ");
        memset(username, 0, MAX_USERNAME_LENGTH);
        fgets(username, MAX_USERNAME_LENGTH - 1, stdin);
        username[strlen(username) - 1] = '\0';

        printf("Enter your password: ");
        memset(password, 0, MAX_PASSWORD_LENGTH);
        fgets(password, MAX_PASSWORD_LENGTH - 1, stdin);
        password[strlen(password) - 1] = '\0';

        printf("username read: %s\n", username);
        printf("password read: %s\n", password);

        packetFC.type = 0;
        strcpy(packetFC.username, username);
        strcpy(packetFC.password, password);
        sendBuffer(sockfd, (char *) &packetFC, sizeof(packetFromClient));

        int no_received = receiveBuffer(sockfd, (char *) &packetFS, sizeof(packetFromServer));
        if (no_received == 0) {
            close(sockfd);
            return 1;
        }

        printf("Message from server: %s\n", packetFS.message);
        if (strcmp(packetFS.message, "Accept") == 0) {
            break;
        }
    }
    while (1) {
        printf("What do you want to do?\n");
        printf("If you want to give a set of primes number, press 0, otherwhise press 1\n");
		tmp_fds = read_fds;
        ret = select(sockfd + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select");

        if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
            
            int decision = -1;
            scanf("%d", &decision);
            if (decision == 0) {
                printf("Enter the number of numbers you want to test if prime\n");

                while(1) {
                    scanf("%d", &packetFC.noNumbers);
                    if (packetFC.noNumbers < MAX_NO_NUMBERS) break;
                    printf("Enter less numbers\n");
                }
                
                for (int it = 0; it < packetFC.noNumbers; it++) {
                    printf("Enter the %d-th number: ", it);
                    scanf("%d", &packetFC.numbers[it]);
                }

                packetFC.type = 1;
                sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));

            } else if (decision == 1) {
                printf("ENTER THE COMMAND YOU WANT TO GIVE\n");
                memset(buffer, 0, BUFLEN);
                //fgets(buffer, BUFLEN - 1, stdin);
                scanf("%s", buffer);

                if (strcmp(buffer, "quit") == 0) {
                    break;
                } else {
                    printf("Don't know what to do with this command\n");
                }
            } else {
                printf("Don't know what to do\n");
            }

        } else if (FD_ISSET(sockfd, &tmp_fds)) {
            int no_received = receiveBuffer(sockfd, (char *)&packetFS, sizeof(packetFromServer));
            if (no_received == 0) break;

            if (strcmp(packetFS.message, "Nu mi-ai dat niciun nr prim\n") == 0) {
                printf("%s\n", packetFS.message);
            } else {
                printf("Message from server: %s\n", packetFS.message);
                printf("Those are the prime numbers the server sent\n");
                for (int it = 0; it < packetFS.noNumbers; it++) {
                    printf("%d ", packetFS.primesNumbers[it]);
                }
                printf("\n\n");
            }
        }
    }

    close(sockfd);

    return 0;
}