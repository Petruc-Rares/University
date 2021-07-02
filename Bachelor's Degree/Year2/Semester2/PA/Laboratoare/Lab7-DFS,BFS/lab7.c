#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char id;
    struct Node *next;
} Node;
 
typedef struct NodeList {
    Node *head;
} NodeList;
 
typedef struct Graph
{
    int noNodes;
    NodeList *lists;
} Graph;
 
Node *createNode(char id) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}
 
Graph* createGraph(int noNodes) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->noNodes = noNodes;
 
    graph->lists = (NodeList*) malloc(noNodes * sizeof(NodeList));

    int it;
    for (it = 0; it < noNodes; it++) graph->lists[it].head = NULL;
 
    return graph;
}
 
void addEdge(Graph* graph, Node *nodeSrc, Node *nodeDst) {
    Node *newNode = createNode(nodeDst->id);
    newNode->next = graph->lists[nodeSrc->id - 65].head;
    graph->lists[nodeSrc->id - 65].head = newNode;
}

void printGraph(Graph* graph) {
    int it;
    for (it = 0; it < graph->noNodes; it++) {
        struct Node* crtList = graph->lists[it].head;
        printf("\n Adjacency list of vertex %c\n head ", it+65);
        while (crtList) {
            printf("-> %c", crtList->id);
            crtList = crtList->next;
        }
        printf("\n");
    }
}

char *colorToText(char colorIdx) {
    switch (colorIdx) {
        case 0:
            return "alb";
            break;
        case 1:
            return "gri";
            break;
        case 2:
            return "negru";
            break;

        default:
            return "alta culoare";
    }
}

int time = 0;

void explore(int i, Graph *graph, char *color, int *parent, int *d, int *f) {
    d[i] = ++time;
    color[i] = 1;

    printf("c(%c) = %s, d(%c) = %d, p(%c) = %c\n\n", i+65, colorToText(color[i]), i+65, d[i], i+65, parent[i]+65);

    Node *succesors = graph->lists[i].head;
    while (succesors) {
        if (color[succesors->id - 65] == 0) {
            parent[succesors->id - 65] = i;
            explore(succesors->id - 65, graph, color, parent, d, f);
        }
        succesors = succesors ->next;
    }
    color[i] = 2;
    f[i] = ++time;

    printf("c(%c) = %s, f(%c) = %d\n\n", i+65, colorToText(color[i]), i+65, f[i]);

    return;
}

Graph *graphLab() {
    int noNodes = 10;
    Graph *graph = createGraph(noNodes);
    Node *nodeA = createNode('A');
    Node *nodeB = createNode('B');
    Node *nodeC = createNode('C');
    Node *nodeD = createNode('D');
    Node *nodeE = createNode('E');
    Node *nodeF = createNode('F');
    Node *nodeG = createNode('G');
    Node *nodeH = createNode('H');
    Node *nodeI = createNode('I');
    Node *nodeJ = createNode('J');

    addEdge(graph, nodeA, nodeH);
    addEdge(graph, nodeA, nodeC);
    addEdge(graph, nodeA, nodeB);
    addEdge(graph, nodeB, nodeI);
    addEdge(graph, nodeB, nodeH);
    addEdge(graph, nodeB, nodeE);
    addEdge(graph, nodeB, nodeD);
    addEdge(graph, nodeC, nodeD);
    addEdge(graph, nodeD, nodeJ);
    addEdge(graph, nodeD, nodeE);
    addEdge(graph, nodeE, nodeG);
    addEdge(graph, nodeE, nodeF);
    addEdge(graph, nodeF, nodeG);
    addEdge(graph, nodeH, nodeI);
    addEdge(graph, nodeI, nodeA);
    
    return graph;
}

Graph *graphCurs() {
    int noNodes = 12;
    Graph *graph = createGraph(noNodes);
    Node *nodeA = createNode('A');
    Node *nodeB = createNode('B');
    Node *nodeC = createNode('C');
    Node *nodeD = createNode('D');
    Node *nodeE = createNode('E');
    Node *nodeF = createNode('F');
    Node *nodeG = createNode('G');
    Node *nodeH = createNode('H');
    Node *nodeI = createNode('I');
    Node *nodeJ = createNode('J');
    Node *nodeK = createNode('K');
    Node *nodeL = createNode('L');

    addEdge(graph, nodeA, nodeG);
    addEdge(graph, nodeA, nodeB);
    addEdge(graph, nodeB, nodeH);
    addEdge(graph, nodeC, nodeE);
    addEdge(graph, nodeC, nodeD);
    addEdge(graph, nodeE, nodeF);
    addEdge(graph, nodeG, nodeC);
    addEdge(graph, nodeG, nodeB);
    addEdge(graph, nodeH, nodeA);
    addEdge(graph, nodeI, nodeA);
    addEdge(graph, nodeI, nodeK);
    addEdge(graph, nodeI, nodeJ);
    addEdge(graph, nodeJ, nodeK);
    addEdge(graph, nodeK, nodeL);

    return graph;
}

Graph *graphCurs2() {
    int noNodes = 12;
    Graph *graph = createGraph(noNodes);
    Node *nodeA = createNode('A');
    Node *nodeB = createNode('B');
    Node *nodeC = createNode('C');
    Node *nodeD = createNode('D');
    Node *nodeE = createNode('E');
    Node *nodeF = createNode('F');
    Node *nodeG = createNode('G');
    Node *nodeH = createNode('H');
    Node *nodeI = createNode('I');
    Node *nodeJ = createNode('J');
    Node *nodeK = createNode('K');
    Node *nodeL = createNode('L');

    addEdge(graph, nodeA, nodeB);
    addEdge(graph, nodeA, nodeG);
    addEdge(graph, nodeB, nodeH);
    addEdge(graph, nodeC, nodeE);
    addEdge(graph, nodeC, nodeD);
    addEdge(graph, nodeE, nodeF);
    addEdge(graph, nodeG, nodeC);
    addEdge(graph, nodeG, nodeB);
    addEdge(graph, nodeH, nodeA);
    addEdge(graph, nodeI, nodeA);
    addEdge(graph, nodeI, nodeK);
    addEdge(graph, nodeI, nodeJ);
    addEdge(graph, nodeJ, nodeK);
    addEdge(graph, nodeK, nodeL);

    return graph;
}

void dfs(Graph *graph) {
    int noNodes = graph->noNodes;

    char *color = malloc(noNodes * sizeof(char));
    // 0 - alb
    // 1 - gri
    // 2 - negru
    int *parent = malloc(noNodes * sizeof(int));
    int *d = malloc(noNodes * sizeof(int));
    int *f = malloc(noNodes * sizeof(int));

    //printGraph(graph);
    for (int i = 0; i < noNodes; i++) {
        color[i] = 0;
        parent[i] = -1;
    }

    time = 0;
    for (int i = 0; i < noNodes; i++) {
        if (color[i] == 0) {
            explore(i, graph, color, parent, d, f);
        }
    }

    printf("\nLista cu timpii:\n");
    for (int i = 0; i < noNodes; i++) {
        printf("nodul %c are timpul de descoperire %d si timpul de finalizare %d\n", i + 65, d[i], f[i]);
    }
}

void bfs(Graph *graph, int source) {
    int noNodes = graph->noNodes;
    int *parent = malloc (noNodes * sizeof(int));
    int *distance = malloc(noNodes * sizeof(int));
    char *color = malloc(noNodes * sizeof(char));

    for (int i = 0; i < noNodes; i++) {
        parent[i] = -1;
        color[i] = 0;
        distance[i] = RAND_MAX;
    }

    int *queue = malloc(noNodes * sizeof(int));


    int flag2 = 0;  // folosit pentru bfs-ul de la curs

    bfsAgain:
    distance[source] = 0;

    int crtElemIdx = 0;
    int lastElemIdx = 0;
    int newAddedIdx = 0;
    queue[crtElemIdx] = source;
    parent[source] = -1;
    color[source] = 1;
    int flag = 1;

    while (crtElemIdx < lastElemIdx + 1) {
        printf("Q = {");
        for (int i = crtElemIdx; i <= lastElemIdx; i++) {
            printf("%c ", (queue[i] + 65));
        }
        printf("\b}");
        // extragem primul element
        int crtElemId = queue[crtElemIdx]; 

        if (flag == 1) {
            printf(", ");
            for (int i = newAddedIdx; i <= lastElemIdx; i++) {
                printf("d(%c) = %d, ", queue[i]+65, distance[queue[i]]);
            }

            for (int i = newAddedIdx; i <= lastElemIdx; i++) {
                printf("p(%c) = %c, ", queue[i]+65, parent[queue[i]] + 65);
            }

            for (int i = newAddedIdx; i <= lastElemIdx; i++) {
                printf("c(%c) = %s, ", queue[i]+65, colorToText(color[queue[i]]));
            }
            if (crtElemId != source) {
                printf("c(%c) = %s", queue[crtElemIdx - 1] + 65, colorToText(color[queue[crtElemIdx - 1]]));
            }
        } else {
            printf(", c(%c) = %s", queue[crtElemIdx - 1] + 65, colorToText(color[queue[crtElemIdx - 1]]));
        }
        printf("\n");

        flag = 0;

        crtElemIdx++;

        newAddedIdx = lastElemIdx + 1;

        Node *crtElemSuccesors = graph->lists[crtElemId].head;
        while (crtElemSuccesors) {
            if (color[crtElemSuccesors->id - 65] == 0) {
                flag = 1;
                lastElemIdx++;
                distance[crtElemSuccesors->id - 65] = distance[crtElemId] + 1;
                parent[crtElemSuccesors->id - 65] = crtElemId;
                color[crtElemSuccesors->id - 65] = 1;
                queue[lastElemIdx] = crtElemSuccesors->id - 65;
            }
            crtElemSuccesors = crtElemSuccesors->next;
        }

 /*       if (source == 8) {
            printf("crtElemIdx: %d, lastElemIdx: %d, crtElemId: %d\n", crtElemIdx, crtElemId);
            exit(1);
        }*/
        color[crtElemId] = 2;
    }
        printf("Q = {}, c(%c) = %s\n", queue[lastElemIdx] + 65, colorToText(color[queue[noNodes - 1]]));

    if ((noNodes == 12) && (flag2 == 0)) {
        printf("Incepe a doua parcurgere BFS:\n");
        source = 8;
        flag2 = 1;
        goto bfsAgain;
    }
    
}

int main() {
    printf("\n\nDFS pe graful de la laborator:\n\n");
    dfs(graphLab());
    printf("\n\nDFS pe graful de la curs:\n\n");
    dfs(graphCurs());
    printf("\n\nBFS Pe graful de la laborator:\n\n");
    bfs(graphLab(), 0);
    printf("\n\nBFS Pe graful de la curs:\n\n");
    bfs(graphCurs2(), 0);
}