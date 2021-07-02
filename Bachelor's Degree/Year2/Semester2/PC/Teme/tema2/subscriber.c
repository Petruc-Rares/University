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
#include <netinet/tcp.h>
#include "common_utils.h"

#define BUFLEN 1551
#define SIZE_SERVER_PACKET 1572

void usage(char *file) {
    fprintf(stderr, "Usage: %s <ID_CLIENT> <IP_SERVER> (dotted decimal) <PORT_SERVER>\n", file);
    exit(0);
}

char *typeToString(unsigned char type) {
    switch (type) {
        case 0:
            return "INT";
            break;
        case 1:
            return "SHORT_REAL";
            break;
        case 2:
            return "FLOAT";
            break;
        case 3:
            return "STRING";
            break;
        default:
            return "UNKNOWN_TYPE";
    }
}

int main(int argc, char* argv[]) {
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    /* TODO TCP SUBSCRIBER/CLIENT */
    if (argc != 4) {
        usage(argv[0]);
    }

    int sockfd, n, ret;
    struct sockaddr_in serv_addr;
    char buffer[SIZE_SERVER_PACKET];
    char bufferClient[MESSAGE_CLIENT_LEN];

    packetServer* packetFromServer;
    packetClient packetFromClient;

    fd_set read_fds;
	fd_set tmp_fds;

    // se goleste multimea de descriptori de citire (read_fds)
    // si cea temporara (tmp_fds)
    FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    DIE(sockfd < 0, "socket");

    int flag = 1;
    int result = setsockopt(sockfd,            /* socket affected */
                        IPPROTO_TCP,     /* set option at TCP level */
                        TCP_NODELAY,     /* name of option */
                        (char *) &flag,  /* the cast is historical cruft */
                        sizeof(int)); 
    DIE(result<0, "NEAGLE failed on subscriber");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[3]));
    ret = inet_aton(argv[2], &serv_addr.sin_addr);
    DIE(ret == 0, "inet_aton in subscriber");

    ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    DIE(ret < 0, "connect failed in subscriber");

    sprintf(packetFromClient.payload, "CLIENT_ID: %s %ld", argv[1], strlen(argv[1]));
    packetFromClient.payload[strlen(packetFromClient.payload)] = '\0';

    n = send(sockfd, &packetFromClient, sizeof(packetClient), 0);
    DIE(n < 0, "send failed in subscriber");

    // adaugam cei doi socketi, cel de la tastatura si cel al serverului
    FD_SET(sockfd, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    int nRecvd = 0;
    int sizeServerPacket = sizeof(packetServer);

    while (1) {
		tmp_fds = read_fds;
        ret = select(sockfd + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select");

        if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
            // se citeste de la tastatura
            memset(bufferClient, 0, MESSAGE_CLIENT_LEN);
            fgets(bufferClient, MESSAGE_CLIENT_LEN - 1, stdin);

            bufferClient[strcspn(bufferClient, "\n")] = 0;

            memcpy(packetFromClient.payload, bufferClient, MESSAGE_CLIENT_LEN);

            // se trimite mesaj la server
            n = send(sockfd, &packetFromClient, sizeof(packetClient), 0);
            DIE(n < 0, "send failed in subscriber");

            if (strncmp(packetFromClient.payload, "exit", 4) == 0) {
                break;
            }
        } else if (FD_ISSET(sockfd, &tmp_fds)) {
            // se primeste mesaj de la server

            while(nRecvd < sizeServerPacket) {
                nRecvd += recv(sockfd, buffer + nRecvd,  sizeServerPacket, 0);
                
                // exit de la server
                if (nRecvd == 0) {
                    close(sockfd);
                    return 0;
                }
                DIE(nRecvd < 0, "recv failed in getting message from server");
            }
            
            packetFromServer = (packetServer *)buffer;
            if (packetFromServer->flag == SUBSCRIPTION_RELATED) {
                // daca e confirmare de subscribe sau unsubscribe
                printf("%s\n", packetFromServer->payload);
            } else if (packetFromServer->flag == UDP_MESSAGE) {
                printf("%s:%hu - %s - %s - %s\n", inet_ntoa(packetFromServer->conexionInfo.sin_addr),
                                                            ntohs(packetFromServer->conexionInfo.sin_port),
                                                            packetFromServer->topic,
                                                            typeToString(packetFromServer->tipDate),
                                                            packetFromServer->payload);
            } else if (packetFromServer->flag == NON_ACK) {
                // exit from server or a nack
                break;
            }

            // reseteaza valorile calculate anterior
            nRecvd = 0;
            memset(buffer, 0, SIZE_SERVER_PACKET);
        }
    }

    close(sockfd);

    return 0;
        
}