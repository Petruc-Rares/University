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

    int sock_player_waiting = -1;

    Node *listGames = NULL;

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

                    if (sock_player_waiting == -1) {
                        printf("Avem un jucator in lobby\n");
                        sock_player_waiting = newsockfd;
                        continue;
                    }
                    // found a pair for the player waiting
                    // create a new game
                    Info info;
                    info.sockfd_player1 = sock_player_waiting;
                    info.sockfd_crtPlayer = sock_player_waiting;
                    info.sockfd_player2 = newsockfd;
                    for (int k = 0; k < 3; k++) {
                        for (int p = 0; p < 3; p++) {
                            info.table[k][p] = 0;
                        }
                    }
                    Node *node = createNode(info);
                    addFrontNode(&listGames, node);
                    printf("Pereche realizata intre %d si %d\n", sock_player_waiting, newsockfd);
                    printf("Se joaca %d jocuri in acest moment\n", sizeList(listGames));

                    memcpy(packetFS.table, info.table, sizeof(info.table));
                    char messageToClient[BUFLEN];
                    strcpy(messageToClient, "Make a move!");
                    strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                    sendBuffer(sock_player_waiting, (char *) &packetFS, sizeof(packetFromServer));

                    strcpy(messageToClient, "Look at the other player move!");
                    strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                    sendBuffer(newsockfd, (char *)&packetFS, sizeof(packetFromServer));

                    // next player will need some other person to play with
                    sock_player_waiting = -1;
                } else {
                    // s-au primit date pe unul din socketii de client,
                    // asa ca serverul trebuie sa le receptioneze

                    int no_received = receiveBuffer(i, (char *)&packetFC, sizeof(packetFC));
                    if (no_received == 0) {
                        printf("S-au primit 0 bytes de la sock_fd: %d\n", i);
                        // client disconnected or game has been won/lost by he/she
                        Node *game = findPlayerBySocket(listGames, i);
                        if (game == NULL) {
                            FD_CLR(i, &read_fds);
                            continue;
                        }

                        int toAnnounceSocket = -1;
                        printf("sockets: %d %d\n", game->content.sockfd_player1, game->content.sockfd_player2);
                        if (i == game->content.sockfd_player1) toAnnounceSocket = game->content.sockfd_player2;
                        else if (i == game->content.sockfd_player2) toAnnounceSocket = game->content.sockfd_player1;

                        printf("Se trimite mesaj de disconnect catre: %d\n", toAnnounceSocket);
                        char messageToClient[BUFLEN];
                        strcpy(messageToClient, "Disconnect!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(toAnnounceSocket, (char *)&packetFS, sizeof(packetFromServer));


                        FD_CLR(game->content.sockfd_player1 ,&read_fds);
                        FD_CLR(game->content.sockfd_player2 ,&read_fds);
                        deleteNode(&listGames, game);
                        continue;
                    }


                    char messageToClient[BUFLEN];
                    memset(messageToClient, 0, BUFLEN);
                    // verificam daca jucatorul cu socket-ul i asociat
                    // se afla intr-un joc
                    Node *game = findPlayerBySocket(listGames, i);
                    if (game == NULL) {
                        for (int itR = 0; itR < 3; itR++) {
                            for (int itC = 0; itC < 3; itC++) {
                                packetFS.table[itR][itC] = 1;
                            }
                        }
                        strcpy(messageToClient, "Unfortunately game did not start\n");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        continue;
                    }

                    packetFC.idxRow -= 48;
                    packetFC.idxCol -= 48;
                    printf("idxRow: %d, idxCol: %d\n", packetFC.idxRow, packetFC.idxCol);

                    printf("Game found!\n");

                    if ((packetFC.idxRow < 0) || (packetFC.idxRow > 2) ||
                     (packetFC.idxCol < 0) || (packetFC.idxCol > 2)) {

                        printf("Invalid position\n");
                        memcpy(packetFS.table, game->content.table, sizeof(game->content.table));
                        strcpy(messageToClient, "Invalid position\n");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        continue;
                    }

                    Info gameContent = game->content;
                    if (gameContent.table[packetFC.idxRow][packetFC.idxCol]) {
                        printf("Already completed cell, try another cell\n");
                        memcpy(packetFS.table, game->content.table, sizeof(game->content.table));
                        strcpy(messageToClient, "Already completed cell, try another cell\n");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        continue;
                    }

                    if (gameContent.sockfd_crtPlayer == i) {
                        printf("%d %d\n", gameContent.sockfd_crtPlayer, gameContent.sockfd_player1);
                        if (gameContent.sockfd_crtPlayer ==  gameContent.sockfd_player1) {
                            printf("Se face 1 aici\n");
                            gameContent.table[packetFC.idxRow][packetFC.idxCol] = 1;
                            gameContent.sockfd_crtPlayer = gameContent.sockfd_player2;

                            memcpy(packetFS.table, gameContent.table, sizeof(gameContent.table));
                            strcpy(messageToClient, "Make a move!");
                            strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                            sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                            strcpy(messageToClient, "Look at the other player move!");
                            strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                            sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));
                        } else if  (gameContent.sockfd_crtPlayer ==  gameContent.sockfd_player2) {
                            gameContent.table[packetFC.idxRow][packetFC.idxCol] = 2;
                            gameContent.sockfd_crtPlayer = gameContent.sockfd_player1;

                            memcpy(packetFS.table, gameContent.table, sizeof(gameContent.table));
                            strcpy(messageToClient, "Make a move!");
                            strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                            sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                            strcpy(messageToClient, "Look at the other player move!");
                            strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                            sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));
                        }
                    } else {
                        memcpy(packetFS.table, game->content.table, sizeof(game->content.table));
                        strcpy(messageToClient, "Not your turn\n");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        continue;
                    }

                    game->content = gameContent;
                    for (int k = 0; k < 3; k++) {
                        for (int j = 0; j < 3; j++) {
                            if(gameContent.table[k][j] == 1) {
                                printf("x");
                            } else if (gameContent.table[k][j] == 2) {
                                printf("0");
                            } else if (gameContent.table[k][j] == 0) {
                                printf("_");
                            }
                        }
                        printf("\n");
                    }

                    int nocol01 = 0;
                    int nocol02 = 0;
                    int nocol11 = 0;
                    int nocol12 = 0;
                    int nocol21 = 0;
                    int nocol22 = 0;
                    // verificam coloanele
                    for (int it = 0; it < 3; it++) {
                        if (gameContent.table[it][0] == 1) nocol01++;
                        if (gameContent.table[it][0] == 2) nocol02++;
                        if (gameContent.table[it][1] == 1) nocol11++;
                        if (gameContent.table[it][1] == 2) nocol12++;
                        if (gameContent.table[it][2] == 1) nocol21++;
                        if (gameContent.table[it][2] == 2) nocol22++;
                    }

                    if ((nocol01 == 3) || (nocol11 == 3) || (nocol21 == 3)) {
                        strcpy(messageToClient, "You Won!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        strcpy(messageToClient, "You Lost!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        printf("A castigat player 1 cu sockfd: %d\n", gameContent.sockfd_player1);
                        deleteNode(&listGames, game);

                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);

                        continue;
                    } else if ((nocol02 == 3) || (nocol12 == 3) || (nocol22 == 3)) {
                        strcpy(messageToClient, "You Won!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        strcpy(messageToClient, "You Lost!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        printf("A castigat player 2 cu sockfd: %d\n", gameContent.sockfd_player2);
                        deleteNode(&listGames, game);

                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);

                        continue;
                    }

                    int norow01 = 0;
                    int norow02 = 0;
                    int norow11 = 0;
                    int norow12 = 0;
                    int norow21 = 0;
                    int norow22 = 0;
                    // verificam liniile
                    for (int j = 0; j < 3; j++) {
                        if (gameContent.table[0][j] == 1) norow01++;
                        if (gameContent.table[0][j] == 2) norow02++;
                        if (gameContent.table[1][j] == 1) norow11++;
                        if (gameContent.table[1][j] == 2) norow12++;
                        if (gameContent.table[2][j] == 1) norow21++;
                        if (gameContent.table[2][j] == 2) norow22++;
                    }

                    if ((norow01 == 3) || (norow11 == 3) || (norow21 == 3)) {
                        strcpy(messageToClient, "You Won!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        strcpy(messageToClient, "You Lost!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        printf("A castigat player 1 cu sockfd: %d\n", gameContent.sockfd_player1);
                        deleteNode(&listGames, game);

                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);
                        continue;
                    } else if ((norow02 == 3) || (norow12 == 3) || (norow22 == 3)) {
                        strcpy(messageToClient, "You Won!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        strcpy(messageToClient, "You Lost!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        printf("A castigat player 2 cu sockfd: %d\n", gameContent.sockfd_player2);
                        deleteNode(&listGames, game);

                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);

                        continue;
                    }

                    // mai raman diagonalele
                    // diag principala
                    int nodiag01 = 0;
                    int nodiag02 = 0;
                    for (int it = 0; it < 3; it++) {
                        if (gameContent.table[it][it] == 1) nodiag01++;
                        if (gameContent.table[it][it] == 2) nodiag02++;
                    }

                    if (nodiag01 == 3) {
                        strcpy(messageToClient, "You Won!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        strcpy(messageToClient, "You Lost!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        printf("A castigat player 1 cu sockfd: %d\n", gameContent.sockfd_player1);
                        deleteNode(&listGames, game);

                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);
                        
                        continue;
                    } else if (nodiag02 == 3) {
                        strcpy(messageToClient, "You Won!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        strcpy(messageToClient, "You Lost!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        printf("A castigat player 2 cu sockfd: %d\n", gameContent.sockfd_player2);
                        deleteNode(&listGames, game);
                        
                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);
                        continue;                      
                    }

                    // diag. secundara
                    int nodiag11 = 0;
                    int nodiag12 = 0;
                    for (int it = 0; it < 3; it++) {
                        if (gameContent.table[it][2-it] == 1) nodiag11++;
                        if (gameContent.table[it][it] == 2) nodiag12++;
                    }

                    if (nodiag11 == 3) {
                        strcpy(messageToClient, "You Won!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        strcpy(messageToClient, "You Lost!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        printf("A castigat player 1 cu sockfd: %d\n", gameContent.sockfd_player1);
                        deleteNode(&listGames, game);

                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);
                        continue;
                    } else if (nodiag12 == 3) {
                        strcpy(messageToClient, "You Won!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        strcpy(messageToClient, "You Lost!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        printf("A castigat player 2 cu sockfd: %d\n", gameContent.sockfd_player2);
                        deleteNode(&listGames, game);
                        
                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);
                        
                        continue;                      
                    }

                    // if nonone won, maybe all cells are marked
                    int contor = 0;
                    for (int itR = 0; itR < 3; itR++) {
                        for (int itC = 0; itC < 3; itC++) {
                            if (gameContent.table[itR][itC] != 0) {
                                contor++;
                            }
                        }
                    }
                    if (contor == 9) {
                        strcpy(messageToClient, "Tie!");
                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player2, (char *)&packetFS, sizeof(packetFromServer));

                        strncpy(packetFS.message, messageToClient, sizeof(messageToClient));
                        sendBuffer(gameContent.sockfd_player1, (char *)&packetFS, sizeof(packetFromServer));

                        FD_CLR(gameContent.sockfd_player1, &read_fds);
                        FD_CLR(gameContent.sockfd_player2, &read_fds);
                    }
                }
            }
        }
    }

    close(sockfd);

    return 0;
}
