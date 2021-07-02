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
    memset(packetFS.buffer, 0, sizeof(packetFromServer));

    Node *listPeople = NULL;

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

                    memset(packetFS.buffer, 0, BUFLEN);
                    if (strncmp(packetFC.buffer, "CREATE", 6) == 0) {
                        printf("CREATE ENCOUNTERED\n");
                        char username[MAX_USERNAME_LENGTH];
                        char password[MAX_PASSWORD_LENGTH];

                        sscanf(packetFC.buffer, "CREATE %s %s\n", username, password);

                        //printf("se doreste a fi creat\n");
                        //printf("user: %s, lung_user: %ld, password: %s, lung_password: %ld\n", 
                        //username, strlen(username), password, strlen(password));

                        Node *client = findClientByName(listPeople, username);
                        if (client != NULL) {
                            sprintf(packetFS.buffer, "Already taken name");
                            packetFS.code = 1;
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                            continue;
                        }

                        Info newInfo = createInfo(username, password);
                        Node *newClient = createNode(newInfo);
                        addFrontNode(&listPeople, newClient);
                    
                        //fprintf(packetFS.buffer, )
                        sprintf(packetFS.buffer, "Successfully created account");
                        packetFS.code = 0;
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));

                        printList(listPeople);
                    } else if (strncmp(packetFC.buffer, "LOGIN", 5) == 0) {
                        char username[MAX_USERNAME_LENGTH];
                        char password[MAX_PASSWORD_LENGTH];

                        sscanf(packetFC.buffer, "LOGIN %s %s\n", username, password);

                        Node *match = checkCredentials(listPeople, username, password);
                        if ((match == NULL) || (match->content.sockfd != -1)) {
                            packetFS.code = -1;
                            sprintf(packetFS.buffer, "WRONG CREDENTIALS or already connected user OR no account found\n");
                        } else {
                            sprintf(packetFS.buffer, "Authentification succeded\n");
                            packetFS.code = 0;
                            // update sockfd for crt connection
                            match->content.sockfd = i;
                        }
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        printf("After login\n");
                        printList(listPeople);
                    } else if (strncmp(packetFC.buffer, "UPDATE_DESC", 11) == 0) {
                        printf("UPDATE_DESC ENCOUNTERED\n");

                        Node *client = findClientBySocket(listPeople, i);
                        if (client == NULL) {
                            printf("CAN'T FIND A CLIENT FOR THE GIVEN SOCKET\n");
                            packetFS.code = 1;
                            sprintf(packetFS.buffer, "No client associated with this socket\n");
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                            continue;
                        }

                        char description[MAX_DESCRIPTION_LENGTH];
                        sscanf(packetFC.buffer, "UPDATE_DESC %[^\t\n]", description);

                        memcpy(client->content.description, description, MAX_DESCRIPTION_LENGTH);
                        packetFS.code = 0;

                        sprintf(packetFS.buffer, "Update done\n");
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        printf("After update_desc\n");
                        printList(listPeople);

                    } else if (strncmp(packetFC.buffer, "ADD_FRIEND", 10) == 0) {
                        printf("ADD_FRIEND ENCOUNTERED\n");

                        // check if client is authentificated
                        Node *client = findClientBySocket(listPeople, i);
                        if (client == NULL) {
                            printf("CAN'T FIND A CLIENT FOR THE GIVEN SOCKET\n");
                            packetFS.code = 1;
                            sprintf(packetFS.buffer, "CAN'T FIND A CLIENT FOR THE GIVEN SOCKET\n");
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                            continue;
                        }

                        char nameFriend[MAX_USERNAME_LENGTH];
                        sscanf(packetFC.buffer, "ADD_FRIEND %s", nameFriend);

                        // check if friend exists
                        Node *friend = findClientByName(listPeople, nameFriend);
                        if (friend == NULL) {
                            printf("Not existing friend, can't add this\n");
                            packetFS.code = 1;
                            sprintf(packetFS.buffer, "Not existing friend, can't add this\n");
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                            continue;
                        }

                        if ((client->content.crtNoFriends >= MAX_NO_FRIENDS) ||
                            (friend->content.crtNoFriends >= MAX_NO_FRIENDS)) {
                            printf("CLIENT HAS ALREADY REACHED MAXIMUM NUMBER OF USERS AS FRIENDS\n");
                            packetFS.code = 1;
                            sprintf(packetFS.buffer, "Someone of you reached maximum number of friends\n");
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                            continue;
                        }


                        int noCrtClient = client->content.crtNoFriends;
                        int noCrtFriend = friend->content.crtNoFriends;
                        // else add the new friend
                        // first, increase the number of friends
                        client->content.crtNoFriends++;
                        friend->content.crtNoFriends++;
                        // add the name of the friend
                        memcpy(client->content.friendUsernames[noCrtClient], nameFriend, MAX_USERNAME_LENGTH);
                        memcpy(friend->content.friendUsernames[noCrtFriend], client->content.username, MAX_USERNAME_LENGTH);

                        packetFS.code = 0;
                        sprintf(packetFS.buffer, "Succesfully maked you friends\n");
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        printf("After add_friend\n");
                        printList(listPeople);
                    } else if (strncmp(packetFC.buffer, "SHOW_INFO", 9) == 0) {
                        // presupunem ca se pot vizualiza informatii despre un user si daca
                        // cel care le cere nu este autentificat
                        printf("SHOW_INFO encountered\n");

                        char username[MAX_USERNAME_LENGTH];
                        sscanf(packetFC.buffer, "SHOW_INFO %s", username);

                        printf("username read: %s\n", username);

                        Node *client = findClientByName(listPeople, username);
                        
                        if (client == NULL) {
                            printf("Did not found client you searched for\n");
                            packetFS.code = 1;
                            sprintf(packetFS.buffer, "Did not found client you searched for\n");
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                            continue;
                        }

                        sprintf(packetFS.buffer, "Clientul cu username-ul: %s are descrierea: %s si prietenii: ",
                         username, client->content.description);

                        int lung_buffer = strlen(packetFS.buffer);
                        for (int it = 0; it < MAX_NO_FRIENDS; it++) {
                            if (strlen(client->content.friendUsernames[it]) != 0) {
                                //printf("E un prieten\n");
                                memcpy(packetFS.buffer + lung_buffer, client->content.friendUsernames[it], strlen(client->content.friendUsernames[it]));
                                memcpy(packetFS.buffer + strlen(packetFS.buffer), " \0", 2);
                                lung_buffer = strlen(packetFS.buffer);
                            }
                        }

                        printf("packetFS.buffer: %s\n", packetFS.buffer);

                        packetFS.code = 0;
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                        
                    } else if (strncmp(packetFC.buffer, "SHOW_PEOPLE_I_MIGHT_KNOW", 24) == 0) {
                        printf("SHOW_PEOPLE_I_MIGHT_KNOW encountered\n");
                        Node *client = findClientBySocket(listPeople, i);
                        if (client == NULL) {
                            printf("Client possible not logged in\n");
                            packetFS.code = -1;
                            sprintf(packetFS.buffer, "You might not be authentificated\n");
                            sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                            continue;
                        }

                        sprintf(packetFS.buffer, "You might know: ");
                        int lengthBuffer = strlen(packetFS.buffer);
                        
                        for (int it = 0; it < client->content.crtNoFriends; it++) {
                            Node *friendClient = findClientByName(listPeople, client->content.friendUsernames[it]);
                            for (int it2 = 0; it2 < friendClient->content.crtNoFriends; it2++) {
                                int itCheck;
                                for (itCheck = 0; itCheck < client->content.crtNoFriends; itCheck++) {
                                    if ((strcmp(friendClient->content.friendUsernames[it2],
                                     client->content.friendUsernames[itCheck]) == 0) || (
                                        strcmp(friendClient->content.friendUsernames[it2],
                                     client->content.username) == 0
                                     )) {
                                        break;
                                    }
                                }
                                if (itCheck == client->content.crtNoFriends) {
                                    //printf("No direct friend\n");
                                    memcpy(packetFS.buffer + lengthBuffer, friendClient->content.friendUsernames[it2],
                                     strlen(friendClient->content.friendUsernames[it2]));
                                    memcpy(packetFS.buffer + strlen(packetFS.buffer) , " \0",
                                     2);
                                     lengthBuffer = strlen(packetFS.buffer);
                                }
                            }
                        }

                        packetFS.code = 0;
                        sendBuffer(i, (char *)&packetFS, sizeof(packetFromServer));
                    } else if (strncmp(packetFC.buffer, "LOGOUT", 6) == 0) {
                        Node *client = findClientBySocket(listPeople, i);
                        if (client == NULL) {
                            // TODO
                            continue;
                        }
                        client->content.sockfd = -1;
                    }
                }
            }
        }
    }

    close(sockfd);

    return 0;
}
