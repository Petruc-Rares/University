#include "server_utils_structures.h"
#include "server_utils.h"
#include "common_utils.h"
#include <string.h>
#include <sys/select.h>
#include "unistd.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <math.h>

#define MAX_LEN 100
#define TOPIC_LEN 51
#define BUFLEN 1551

Subscriber *findSubscriberBySocket(Subscriber *subscribers, int sockfd) {
    Subscriber *foundSubscriber = NULL;
    while (subscribers) {
        if (subscribers->sockFd == sockfd) {
            foundSubscriber = subscribers;
            break;
        }
        subscribers = subscribers->nextSubscriber;
    }

    return foundSubscriber;
}

Subscriber *findSubscriberById(Subscriber *subscribers, char *id) {
    Subscriber *foundSubscriber = NULL;
    while (subscribers) {
        if (strcmp(subscribers->clientID, id) == 0) {
            foundSubscriber = subscribers;
            break;
        }
        subscribers = subscribers->nextSubscriber;
    }
    return foundSubscriber;
}

TopicInfo *searchTopic(TopicInfo *topics, char *topicName) {
    TopicInfo *foundTopic = NULL;
    while (topics) {
        if (strcmp(topicName, topics->topic) == 0) {
            foundTopic = topics;
            break;
        }
        topics = topics->nextTopic;
    }
    return foundTopic;
}
  
void printTopics(TopicInfo *topicsInfo) {
    printf("\n\nPRINT TOPICS\n\n");
    while (topicsInfo) {
        printf("topicName: %s\n\n", topicsInfo->topic);
        printf("subscribers:\n\n");
        SubscriberInfo *subscribersInfo = topicsInfo->ListSubscriberInfo;
        while (subscribersInfo != NULL) {
            Subscriber *crtSub = subscribersInfo->subscriber;
            printf("current subscriber: %d %s %hhu\n", crtSub->sockFd, crtSub->clientID, crtSub->isConnected);
            printf("sf = %hhu\n", subscribersInfo->sf);
            subscribersInfo = subscribersInfo->nextInfo;
        }
        topicsInfo = topicsInfo->nextTopic;
    }
}

void printSubscribers(Subscriber *subscribers) {
    printf("\n\nPRINT SUBSCRIBERS\n\n");

    while(subscribers) {
        printf("crtSubscriber: %s %d %hhu\n", subscribers->clientID, subscribers->sockFd, subscribers->isConnected);
        subscribers = subscribers->nextSubscriber;
    }
}

int nRecvd = 0;

void sendMessageTCP(char *buffer, int i, Subscriber **listSubscribers,
                fd_set *set_read_fds, packetClient **pointerPacketFromClient,
                packetServer *pointerPacketFromServer, TopicInfo **pointerTopicsInfo) {
    int sizePacketClient = sizeof(packetClient);

    while(nRecvd < sizePacketClient) {
        nRecvd += recv(i, buffer + nRecvd, sizePacketClient, 0);
        DIE(nRecvd < 0, "recv failed in getting message from TCP client");
    }


    char serverMessage[MAX_LEN];
    char topicName[TOPIC_LEN];
    unsigned char sf;

    packetClient *packetFromClient = *pointerPacketFromClient;
    packetServer packetFromServer = *pointerPacketFromServer;

    if ((nRecvd == 0) || (strncmp(buffer, "exit", 4) == 0)) {
        nRecvd = 0;
        close(i);
        Subscriber *searchedSubscriber = findSubscriberBySocket((*listSubscribers), i);
        DIE(searchedSubscriber == NULL, "nu a fost gasit subscriberul pentru care trebuia scos clientul");
        searchedSubscriber->isConnected = 0;

        // se scoate din multimea de citire socketul inchis
        FD_CLR(i, set_read_fds);

        int no = sprintf(serverMessage, "Client %s disconnected.", searchedSubscriber->clientID);
        DIE(no < 0, "sprintf failed");
        printf("%s\n", serverMessage);
    } else {                    
        // make it back to 0, after the whole package has been received
        nRecvd = 0;
        if (strncmp(buffer, "CLIENT_ID:", 10) == 0) {
            // verifica ca niciun alt client nu are acelasi id
            char idClient[11];
            int sizeIdClient;
            memset(idClient, '\0', 11);
            int no = sscanf(buffer, "CLIENT_ID: %s %d", idClient, &sizeIdClient);
            DIE(no==EOF, "sscanf failed, maybe expected different string");
            idClient[strlen(idClient)] = '\0';

            Subscriber *searchedSubscriberID = findSubscriberById((*listSubscribers), idClient);
            if (searchedSubscriberID != NULL) {
                // even someone tries to reconnect, even conflict of id's
                // check conflict (someone with the same id is already connected)
                if (searchedSubscriberID->isConnected == 1) {
                    // remove it's socket
                    FD_CLR(i, set_read_fds);
                    packetFromServer.flag = NON_ACK;
                    int n = send(i, &packetFromServer, sizeof(packetServer), 0);                          
                    DIE(n < 0, "failed to send to duplicated client");
                    close(i);
                    printf("Client %s already connected.\n", idClient);
                } else {
                    // reconnect
                    searchedSubscriberID->isConnected = 1;
                    // inlocuieste socketul anterior
                    searchedSubscriberID->sockFd = i;
                    FD_SET(i, set_read_fds);
                    packetFromServer.flag = ACK;
                    int n = send(i, &packetFromServer, sizeof(packetServer), 0);
                    DIE(n < 0, "failed to send ack");
                    // trimite toate mesajele pentru topicurile la care era abonat cu sf = 1
                    PacketsWaiting *packets = searchedSubscriberID->toBeReceived;
                    while (packets != NULL) {
                        n = send(searchedSubscriberID->sockFd, &packets->m, sizeof(packetServer), 0);
                        DIE(n < 0, "send of not-delivered packages failed");
                        packets = packets->nextPackage;
                    }
                    searchedSubscriberID->toBeReceived = NULL;
                    int no = sprintf(serverMessage, "New client %s connected from %s:%hu", idClient, inet_ntoa(searchedSubscriberID->conexionInfo.sin_addr), searchedSubscriberID->conexionInfo.sin_port);
                    DIE(no < 0, "sprintf failed");
                    printf("%s\n", serverMessage);
                }
                // remove it's entry from the list of subscribers
                Subscriber *aux = (*listSubscribers);
                (*listSubscribers) = (*listSubscribers)->nextSubscriber;
                free(aux);
                return;
            }
            // otherwise brand new connection
            Subscriber *searchedSubscriber = findSubscriberBySocket((*listSubscribers), i);

            DIE(searchedSubscriber == NULL, "no subscriber found after socket");

            packetFromServer.flag = ACK;
            int n = send(i, &packetFromServer, sizeof(packetServer), 0);
            DIE(n < 0, "send ack failed for new connection");

            memcpy(searchedSubscriber->clientID, idClient, strlen(idClient) + 1);
            searchedSubscriber->clientID[sizeIdClient] = '\0';

            no = sprintf(serverMessage, "New client %s connected from %s:%hu", idClient, inet_ntoa(searchedSubscriber->conexionInfo.sin_addr), searchedSubscriber->conexionInfo.sin_port);
            DIE(no < 0, "sprintf failed");

            printf("%s\n", serverMessage);
        } else if (strncmp(buffer, "subscribe", 9) == 0) {
            packetFromClient = (packetClient *)buffer;

            int no = sscanf(packetFromClient->payload, "subscribe %s %hhu", topicName, &sf);
            DIE(no==EOF, "sscanf failed, maybe expected different string");

            TopicInfo *foundTopic = searchTopic(*(pointerTopicsInfo), topicName);
            Subscriber *currentSubscriber = findSubscriberBySocket((*listSubscribers), i);

            SubscriberInfo *newSubscriberInfo = malloc(sizeof(SubscriberInfo));
            DIE(newSubscriberInfo == NULL, "failed to add subscriber info");
            newSubscriberInfo->subscriber = currentSubscriber;
            newSubscriberInfo->sf = sf;
            if (foundTopic == NULL) {
                // noone subscribed here before
                TopicInfo *newTopic = malloc(sizeof(TopicInfo));
                DIE(newTopic == NULL, "failed to add a new topic");
                // add the new subscriber to the new topic
                newTopic->ListSubscriberInfo = newSubscriberInfo;
                newTopic->nextTopic = *(pointerTopicsInfo);
                *(pointerTopicsInfo) = newTopic;
                
                memcpy(newTopic->topic, topicName, strlen(topicName));
            } else {
                SubscriberInfo *subscriberInfo = foundTopic->ListSubscriberInfo;
                while(subscriberInfo) {
                    if (subscriberInfo->subscriber->sockFd == i) {
                        fprintf(stderr, "There is already a subscription from this account");
                        return ;
                    }
                    subscriberInfo = subscriberInfo->nextInfo;
                }
                // there are already some subscribers interested in the topic
                newSubscriberInfo->nextInfo = foundTopic->ListSubscriberInfo;
                foundTopic->ListSubscriberInfo = newSubscriberInfo;
            }

            packetFromServer.flag = SUBSCRIPTION_RELATED;
            char *messageSubscription = "Subscribed to topic.";
            memcpy(packetFromServer.payload, messageSubscription, strlen(messageSubscription) + 1);
            int n = send(i, &packetFromServer, sizeof(packetServer), 0);
            DIE(n < 0, "send of subscribe message from client failed");
        } else if (strncmp(buffer, "unsubscribe", 11) == 0) {
            packetFromClient = (packetClient *)buffer;
            
            int no = sscanf(packetFromClient->payload, "unsubscribe %s", topicName);
            DIE(no==EOF, "sscanf failed, maybe expected different string");

            TopicInfo *foundTopic = searchTopic(*(pointerTopicsInfo), topicName);
            DIE(foundTopic == NULL, "nu s-a gasit topicul inca");

            // remove the subscriber from the topic
            SubscriberInfo *it = foundTopic->ListSubscriberInfo;
            SubscriberInfo *prev = NULL;

            // if first node is one looked for
            if ((it != NULL) && (it->subscriber->sockFd == i)) {
                foundTopic->ListSubscriberInfo = foundTopic->ListSubscriberInfo->nextInfo;
                prev = it; // ca sa nu intre pe cazul de mai jos (if prev == NULL)
                free(it);
            } else while ((it != NULL) && (it->subscriber->sockFd != i)) {
                prev = it;
                it = it->nextInfo;
            }

            DIE(prev == NULL, "Nu poate sa se dea unsubscribe\n");
            DIE(it == NULL, "nu s-a gasit subscriberul");

            prev->nextInfo = it->nextInfo;

            packetFromServer.flag = SUBSCRIPTION_RELATED;
            char *messageSubscription = "Unsubscribed from topic.";
            memcpy(packetFromServer.payload, messageSubscription, strlen(messageSubscription) + 1);
            int n = send(i, &packetFromServer, sizeof(packetServer), 0);
            DIE(n < 0, "send of unsubscribe from server failed");
        } else {
            fprintf(stderr, "UNKNOWN REQUEST FROM TCP CLIENT");
        }
    }
}

Subscriber *createSubscriber(int newsockfd, Subscriber *subscribers, struct sockaddr_in cli_addr) {
    Subscriber *newSubscriber = malloc(sizeof(Subscriber));
    DIE(newSubscriber == NULL, "failed to add a new subscriber");
    memset(newSubscriber->clientID, 0, 11);
    newSubscriber->isConnected = 1;
    newSubscriber->sockFd = newsockfd;
    newSubscriber->toBeReceived = NULL;
    newSubscriber->nextSubscriber = subscribers;
    newSubscriber->conexionInfo = cli_addr;

    return newSubscriber;
}

packetServer createServerSubResponse(struct sockaddr_in serv_addr, char tip_date, char topic[51], char content[1501]) {
    packetServer packetFromServer;
    packetFromServer.flag = UDP_MESSAGE;
    packetFromServer.conexionInfo = serv_addr;
    packetFromServer.tipDate = tip_date;
    memset(packetFromServer.topic, 0, TOPIC_LEN);
    memcpy(packetFromServer.topic, topic, strlen(topic) + 1);
    memset(packetFromServer.payload, 0, CONTENT_LEN);
    memcpy(packetFromServer.payload, content, strlen(content) + 1);
    return packetFromServer;
}

void sendSubResponse(packetServer packetFromServer, TopicInfo *foundTopic) {
    SubscriberInfo *subscribersTopic = foundTopic->ListSubscriberInfo;
    while (subscribersTopic) {
        Subscriber *crtSubscriberTopic = subscribersTopic->subscriber;
        unsigned char crtSf = subscribersTopic->sf;
        if (crtSubscriberTopic->isConnected == 1) {
            int n = send(crtSubscriberTopic->sockFd, &packetFromServer, sizeof(packetServer), 0);
            DIE(n < 0, "failed to send message to subscribed client");
        } else if (crtSf == 1) {
            // daca crtSubscriberTopic e deconectat si are store and forward activat
            // memoreaza in lista lui cand se va reconecta
            PacketsWaiting *newPacketWaiting = malloc(sizeof(PacketsWaiting));
            DIE(newPacketWaiting == NULL, "failed to create the queue of packets");
            newPacketWaiting->m = packetFromServer;
            newPacketWaiting->nextPackage = crtSubscriberTopic->toBeReceived;
            crtSubscriberTopic->toBeReceived = newPacketWaiting;
        }
        subscribersTopic = subscribersTopic->nextInfo;
    }
}

char *convertContent(char *buf, char tip_date) {
    char *content = calloc(BUFLEN, sizeof(char));

    if (tip_date == 0) {
        char sign = buf[51];
        int resultConversion;

        resultConversion = ntohl(*(uint32_t*)(buf+52));

        if (sign == 1) resultConversion *= -1;

        int no = sprintf(content, "%d", resultConversion);
        DIE(no < 0, "sprintf failed");
    } else if (tip_date == 1) {
        short unsigned int resultConversion;

        resultConversion = ntohs(*(uint16_t*)(buf+51));

        int no = sprintf(content, "%.2f", (float) resultConversion / 100);
        DIE(no < 0, "sprintf failed");
    } else if (tip_date == 2) {
        char sign = buf[51];
        int resultConversion;       
        unsigned char division10thPower = buf[56];

        resultConversion = ntohl(*(uint32_t*)(buf+52));
        if (sign == 1) resultConversion *= -1;

        int no = sprintf(content, "%.*f", division10thPower, ((float) resultConversion) / pow(10, division10thPower));
        DIE(no < 0, "sprintf failed");
    }
    else if (tip_date == 3) {
        // get length of content in the datagram
        // null terminated for content shorter than 1500
        size_t content_length = strlen(buf + 51);
        // if content_length exceeds 1500, reduce it to it's max
        // printf("content_length: %d\n", content_length);
        if (content_length >= 1500) {
            content_length = 1500;
        }

        if (content_length != 1500) {
            strcpy(content, buf + 51);
        } else {
            memcpy(content, buf+51, content_length);
            content[1500] = '\0';
        }
    }
    return content;
}

void freeSubscribers(Subscriber **subscribers) {
    while ((*subscribers) != NULL) {
        Subscriber *aux = *subscribers;
        (*subscribers) = (*subscribers)->nextSubscriber;
        free(aux);
    }
    return;
}

void freeTopics(TopicInfo **topicsInfo) {
    while ((*topicsInfo) != NULL) {
        TopicInfo *aux = *topicsInfo;
        (*topicsInfo) = (*topicsInfo)->nextTopic;
        
        SubscriberInfo *auxInfo = aux->ListSubscriberInfo;
        while(auxInfo) {
            SubscriberInfo *auxInfo2 = auxInfo;
            auxInfo = auxInfo->nextInfo;
            free(auxInfo2);
        }
        
        free(aux);
    }
    return;
}