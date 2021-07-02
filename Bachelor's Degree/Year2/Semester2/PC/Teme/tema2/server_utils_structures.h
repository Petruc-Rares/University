#ifndef SERVER_UTILS_STRUCTURES
#define SERVER_UTILS_STRUCTURES

#include <netinet/in.h>
#include "common_utils.h"

#define TOPIC_LEN 51
#define MAX_ID_SIZE 11

typedef struct TopicInfo TopicInfo;
typedef struct SubscriberInfo SubscriberInfo;
typedef struct Subscriber Subscriber;
typedef struct PacketsWaiting PacketsWaiting;

struct Subscriber {
    int sockFd;
    char clientID[MAX_ID_SIZE];
    char isConnected;
    PacketsWaiting *toBeReceived;
    struct sockaddr_in conexionInfo;
    Subscriber *nextSubscriber;
};

struct SubscriberInfo {
    Subscriber *subscriber;
    char sf;
    SubscriberInfo *nextInfo;
};

struct TopicInfo {
    char topic[TOPIC_LEN];
    SubscriberInfo *ListSubscriberInfo;
    TopicInfo *nextTopic;
};

struct PacketsWaiting {
    packetServer m;
    PacketsWaiting *nextPackage;
};

#endif