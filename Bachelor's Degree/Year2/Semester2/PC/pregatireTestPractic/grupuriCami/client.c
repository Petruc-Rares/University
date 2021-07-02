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


    // partea de autentificare
    char name_client[MAX_NAME_LENGTH];
    char group_name[MAX_GROUP_NAME_LENGTH];

    printf("Cum vrei sa fie numele tau?\n");
    fgets(name_client, MAX_NAME_LENGTH - 1, stdin);
    name_client[strlen(name_client) - 1] = '\0';
    printf("Din ce grup vrei sa faci parte?\n");
    fgets(group_name, MAX_GROUP_NAME_LENGTH - 1, stdin);
    group_name[strlen(group_name) - 1] = '\0';
    printf("nume_client: %s, lungime_nume: %ld\n", name_client, strlen(name_client));
    printf("group_name: %s, lungime_group: %ld\n", group_name, strlen(group_name));

    strcpy(packetFC.groupName, group_name);
    strcpy(packetFC.name, name_client);
    packetFC.type = 0;

    // se trimite mesaj la server
    sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));

    while (1) {
		tmp_fds = read_fds;

        printf("Do you want to take part of a group? Press 0 if so\n");
        printf("Do you want to leave the group you are in now? Press 1 if so\n");
        printf("Do you want to join a group? Press 2 if so\n");
        ret = select(sockfd + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select");

        if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
            memset(buffer, 0, BUFLEN);
            fgets(buffer, BUFLEN - 1, stdin);
            int number = atoi(buffer);
            // enter a group
            if (number == 0) {
                printf("What message do you want to send to other participants in the group?\n");
                memset(buffer, 0, MAX_MESSAGE_LENGTH);
                fgets(buffer, MAX_MESSAGE_LENGTH - 1, stdin);

                strcpy(packetFC.message, buffer);
                packetFC.type = 1;
                strcpy(packetFC.groupName, group_name);
                // se trimite mesaj la server
                sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
            } else if (number == 1) {
                // leave a group
                packetFC.type = 2;
                sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
                memset(packetFC.groupName, 0, MAX_GROUP_NAME_LENGTH);
            } else if (number == 2) {
                // enter a new group (if possible, maybe client is already in a group)
                packetFC.type = 0;
                fgets(group_name, MAX_GROUP_NAME_LENGTH - 1, stdin);
                group_name[strlen(group_name) - 1] = '\0';
                strcpy(packetFC.groupName, group_name);
                sendBuffer(sockfd, (char *)&packetFC, sizeof(packetFromClient));
            } else {
                printf("You don't want to take part of a group");
            }

            if (strncmp(buffer, "exit", 4) == 0) {
                break;
            }
        } else if (FD_ISSET(sockfd, &tmp_fds)) {
            int no_received = receiveBuffer(sockfd, (char *)&packetFS, sizeof(packetFromServer));
            if (no_received == 0) break;

            fprintf(stderr, "Received: %s", packetFS.message);
        }
    }

    close(sockfd);

    return 0;
}