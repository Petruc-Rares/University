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
#include <fcntl.h>

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

    while (1) {
		tmp_fds = read_fds;
        packetFC.over = 0;
        printf("What do you want to do? Press 0 for UPLOAD or 1 for DOWNLOAD\n");
        ret = select(sockfd + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select");

        if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
            memset(buffer, 0, BUFLEN);
            fgets(buffer, BUFLEN - 1, stdin);
            buffer[strlen(buffer) - 1] = '\0';

            int noEntered = atoi(buffer);
            if (noEntered == 0) {
                printf("What is the name of the file you want to upload?\n");

                memset(buffer, 0, BUFLEN);
                fgets(buffer, BUFLEN - 1, stdin);
                buffer[strlen(buffer) - 1] = '\0';

                int fd = open(buffer, O_RDONLY);
                if (fd == -1) {
                    printf("I can't find the file, you sure you have it?\n");
                    continue;
                }


                strcpy(packetFC.nameFile, buffer);
                while(1) {
                    char buf[BUFLEN];
                    int bytes_read = read(fd, buf, BUFLEN);
                    printf("Bytes read from client %d\n", bytes_read);
                    packetFC.bytes = bytes_read;
                    packetFC.type = 0;
                    
                    if (bytes_read == 0) {
                        packetFC.over = 1;
                        sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
                        break;
                    }
                    memcpy(packetFC.buffer, buf, BUFLEN);
                    sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));     
                }
            } else if (noEntered == 1) {
                printf("What is the name of the file you want to download?\n");

                memset(buffer, 0, BUFLEN);
                fgets(buffer, BUFLEN - 1, stdin);
                buffer[strlen(buffer) - 1] = '\0';

                packetFC.type = 1;
                strcpy(packetFC.nameFile, buffer);
                packetFC.over = 0;
                
                sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));

                int fd = open(packetFC.nameFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    printf("Can't create file for download in client\n");
                    continue;
                }

                while (1) {
                    receiveBuffer(sockfd, (char *)&packetFS, sizeof(packetFromServer));
                    if (strncmp(packetFS.message, "Internal server error", 21) == 0) break;
                    if (packetFS.over == 1) {
                        packetFC.over = 1;
                        packetFC.type = 1;
                        sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
                        break;
                    }
                    if(packetFC.over == 1) break;
                    write(fd, packetFS.buffer, packetFS.bytes);
                }
            }

            if (strncmp(buffer, "exit", 4) == 0) {
                break;
            }
        } else if (FD_ISSET(sockfd, &tmp_fds)) {
            int no_received = receiveBuffer(sockfd, (char *)&packetFS, sizeof(packetFromServer));
            if (no_received == 0) {

                break;
            }

            fprintf(stderr, "Received: %s", buffer);
        }
    }

    close(sockfd);

    return 0;
}