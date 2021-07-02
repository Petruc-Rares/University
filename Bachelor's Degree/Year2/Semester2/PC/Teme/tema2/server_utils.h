#ifndef SERVER_UTILS_FUNCTIONS
#define SERVER_UTILS_FUNCTIONS

#include "server_utils_structures.h"

// returns subscriber in list @subscribers after socket @sockfd.
// returns NULL if no subscriber in the given list has the sockfd @sockfd
Subscriber *findSubscriberBySocket(Subscriber *subscribers, int sockfd);

// returns subscriber in list @subscribers after id @id.
// returns NULL if no subscriber in the given list has the id @id
Subscriber *findSubscriberById(Subscriber *subscribers, char *id);

// returns topic in list @topics after topic name @topic_name.
// returns NULL if no topic in the given list has the topic name @topic_ame
TopicInfo *searchTopic(TopicInfo *topics, char *topic_name);

// prints topics with additional info in the list @topicInfo
void printTopics(TopicInfo *topicsInfo);

// prints subscribers with additional info in the list @subscribers
void printSubscribers(Subscriber *subscribers);

// sends message to tcp clients @listSubscribers according to info
// received in the buffer
void sendMessageTCP(char *buffer, int i, Subscriber **listSubscribers,
                fd_set *set_read_fds, packetClient **pointerPacketFromClient,
                packetServer *pointerPacketFromServer, TopicInfo **pointerTopicsInfo);

// returns a pointer to a subscriber with sockfd = @newsockfd, nextSubscriber = @subscribers,
// and with the corresponding @cli_addr
Subscriber *createSubscriber(int newsockfd, Subscriber *subscribers, struct sockaddr_in cli_addr);

// creates a packet that the server will send to the clients with neccessary info in response to a subscribe
packetServer createServerSubResponse(struct sockaddr_in serv_addr, char tip_date, char topic[51], char content[1501]);

// sends the packet @packetFromServer to all subscribers subscribed to topic @foundTopic
void sendSubResponse(packetServer packetFromServer, TopicInfo *foundTopic);

// converts the content in @buf, considering @tip_date
char *convertContent(char *buf, char tip_date);


// deallocates memory for list @subscribers
void freeSubscribers(Subscriber **subscribers);

// deallocates memory for list @topicsInfo
void freeTopics(TopicInfo **topicsInfo);

#endif