#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include "server_utils_structures.h"
#include "common_utils.h"
#include "server_utils.h"

#define MAX_LEN 100
#define BUFLEN 1551
#define MAX_CLIENTS 100
#define MAXSIZEPAYLOAD 1700
#define OFFSET_DATA_TYPE 50

void usage(char *file) {
    fprintf(stderr, "Usage: %s <PORT_DORIT>\n", file);
    exit(0);
}


int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    int sockTCP, sockUDP ,newsockfd, portno;
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
    DIE(sockTCP < 0, "TCP socket failed in server");

    portno = atoi(argv[1]);
    DIE(portno == 0, "atoi failed in server after argv[1]");

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(sockTCP, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    DIE(ret < 0, "bind after socket TCP failed");

    ret = listen(sockTCP, MAX_CLIENTS);
    DIE(ret < 0, "listen of socket TCP failed");

    int flag = 1;
    int result = setsockopt(sockTCP,            /* socket affected */
                        IPPROTO_TCP,     /* set option at TCP level */
                        TCP_NODELAY,     /* name of option */
                        (char *) &flag,  /* the cast is historical cruft */
                        sizeof(int)); 
    DIE(result < 0, "Naegle desactivation failed for main TCP socket in server");

    // se adauga noul file descriptor (socketul pe care se asculta conexiuni, subscribe,
    // unsubscribe, etc.)
    FD_SET(sockTCP, &read_fds);
    fdmax = sockTCP;

    sockUDP = socket(AF_INET, SOCK_DGRAM, 0);
    DIE(sockUDP < 0, "socket for UDP failed");

    ret = bind(sockUDP, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    DIE(ret < 0, "bind for socket UDP failed");

    // se adauga noul file descriptor (socketul pe care se primesc datagrame)
    FD_SET(sockUDP, &read_fds);
    fdmax = sockUDP;

    FD_SET(STDIN_FILENO, &read_fds);

    int clients[MAX_LEN];
    char serverMessage[MAX_LEN];
    int index = 0;
    int pos;
    socklen_t len = sizeof(struct sockaddr_in);

    char buf[BUFLEN];
    char bufferTCP[MESSAGE_CLIENT_LEN];

    // this will hold info about the topics and
    // the related subscribers
    TopicInfo *topicsInfo = NULL;
    Subscriber *subscribers = NULL;
    packetClient *packetFromClient;
    packetServer packetFromServer;

    while (1) {
        tmp_fds = read_fds;

        ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
        DIE(ret < 0, "select failed in server");

        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &tmp_fds)) {
                if (i == sockTCP) {
                    clilen = sizeof(cli_addr);
                    newsockfd = accept(sockTCP, (struct sockaddr *)&cli_addr, &clilen);
                    DIE(newsockfd < 0, "accept failed on TCP socket");

                    int result = setsockopt(newsockfd,            /* socket affected */
                        IPPROTO_TCP,     /* set option at TCP level */
                        TCP_NODELAY,     /* name of option */
                        (char *) &flag,  /* the cast is historical cruft */
                        sizeof(int)); 
                    DIE(result < 0, "Naegle desactivation failed on new TCP socket");

                    // se adauga noul socket intors de accept() la multimea descriptorilor de citire
                    FD_SET(newsockfd, &read_fds);
                    if (newsockfd > fdmax) {
                        fdmax = newsockfd;
                    }

                    // se va actualiza lista de subscriberi
                    Subscriber *newSubscriber = createSubscriber(newsockfd, subscribers, cli_addr);
                    subscribers = newSubscriber;
                } else if (i == sockUDP) {
                    // a venit o diagrama pe socketul de udp
                    int bytes_read = recvfrom(sockUDP, buf, BUFLEN, 0, (struct sockaddr *)&serv_addr, &len);
                    DIE(bytes_read < 0, "UDP receive failed");

                    // obtinem lungimea topicului care este terminata cu '\0'
                    // daca topicul e mai scurt de 50 de caractere
                    size_t topic_length = strlen(buf);
                    // daca lungimea returnata depaseste 50 de caractere
                    // se va reduce lungimea la 50
                    if (topic_length >= TOPIC_LEN) {
                        topic_length = TOPIC_LEN;
                    }

                    char topic[TOPIC_LEN];
                    memcpy(topic, buf, topic_length);
                    topic[topic_length] = '\0';

                    TopicInfo *foundTopic = searchTopic(topicsInfo, topic);

                    // noone interested in this topic yet
                    // so go ahead
                    if (foundTopic == NULL) continue;

                    char tip_date;
                    tip_date = buf[OFFSET_DATA_TYPE];

                    char content[CONTENT_LEN];
                    memset(content, 0, CONTENT_LEN);
                    strcpy(content, convertContent(buf, tip_date));
                    memset(buf, 0, BUFLEN);
                    packetFromServer = createServerSubResponse(serv_addr, tip_date, topic, content);
                    sendSubResponse(packetFromServer, foundTopic);
                } else if (i == STDIN_FILENO) {
                    // se citeste de la tastatura
                    memset(buffer, 0, BUFLEN);
                    fgets(buffer, BUFLEN - 1, stdin);

                    if (strcmp(buffer, "exit")) {
                        // trimitem mesaj catre toti clientii conectati la server
                        // sa dezactiveze conexiunile
                        packetFromServer.flag = NON_ACK;
                        for (int i = 4; i <= fdmax; i++) {
                            n = send(i, &packetFromServer, sizeof(packetServer), 0);
                            DIE(n<0, "message to clients for exit failed");
                        }
                        close(sockUDP);
                        close(sockTCP);
                        freeSubscribers(&subscribers);
                        freeTopics(&topicsInfo);
                        return 0;
                    }
                    fprintf(stderr, "Nu a primit exit, comanda necunoscuta\n");
                } else  {
                    // mesaj de la client tcp
                    sendMessageTCP(bufferTCP, i, &subscribers, &read_fds, &packetFromClient, &packetFromServer, &topicsInfo);
                    memset(bufferTCP, 0, sizeof(bufferTCP));
                }
            }
        }
    }

    return 0;
}