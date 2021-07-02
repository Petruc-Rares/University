#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
 
typedef struct Edge {
    int s;
    int d;
    int w;
} Edge;

typedef struct Graph {
    int V, E;
    Edge* edge;
} Graph;
 
struct Graph* createGraph(int V, int E) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));

    graph->V = V;
    graph->E = E;
    graph->edge = (Edge*) malloc(E * sizeof(Edge));

    return graph;
}
 
void printSol(int n, int d[], int p[]) {
    printf("\nnod\t\t d \t\t p\n");

    for (int i = 0; i < n; i++){
		printf("%d \t\t %d \t\t %d\n", i+1, d[i], p[i]+1);
	}
}

int getStartEdge(Graph *graph, int id) {
    for (int i = 0; i < graph->E; i++) {
        if (graph->edge[i].s == id) {
            return i;
        }
    }
    return -1;
}

int getStopEdge(Graph *graph, int id) {
    for (int i = getStartEdge(graph, id) + 1; i < graph->E; i++) {
        if (graph->edge[i].s != id) {
            return i;
        }
    }
    return graph->E;
}

void Dijkstra(Graph *graph) {
    int V = graph->V;
    int E = graph->E;
    int d[V], p[V], visited[V];

    for (int i = 0; i < V; i++) {
        d[i] = INT_MAX;
        p[i] = -1;
        visited[i] = 0;
    }
 
    int source = 0;
    d[source] = 0;

    int nextNode = source;
    int minDistance;

    for (int i = 0; i < V; i++) {
        int j;

        minDistance = INT_MAX;
        for (int it = 0; it < V; it++) {
            if ((visited[it] == 0) && (d[it] < minDistance)) {
                minDistance = d[it];
                nextNode = it;
            }
        }

        visited[nextNode] = 1;

        int s1 = getStartEdge(graph, nextNode);
        if (s1 == -1) continue; // nu avem lista de adiacenta ptr nodul nextNode
        int s2 = getStopEdge(graph, nextNode);

        for (int crtEdge = s1; crtEdge < s2; crtEdge++) {
            int u = graph->edge[crtEdge].s;
            int v = graph->edge[crtEdge].d;
            int w = graph->edge[crtEdge].w;

            if ((visited[v] == 0) && (d[u] + w < d[v])) {
                d[v] = d[u] + w;
                p[v] = u;
            }
        }
    }

    printSol(V, d, p);

    return;
}
 
void BF(Graph* graph) {
    int V = graph->V;
    int E = graph->E;
    int d[V], p[V];
 
    for (int i = 0; i < V; i++) {
        d[i] = INT_MAX;
        p[i] = -1;
    }
 
    int source = 0;

    d[source] = 0;
 
    for (int i = 1; i <= V-1; i++) {
        for (int j = 0; j < E; j++) {
            int u = graph->edge[j].s;
            int v = graph->edge[j].d;
            int w = graph->edge[j].w;

            if ((d[u] != INT_MAX) && (d[u] + w < d[v])) {
                d[v] = d[u] + w;
                p[v] = u;
            }
        }
    }
 

    for (int i = 0; i < E; i++) {
        int u = graph->edge[i].s;
        int v = graph->edge[i].d;
        int w = graph->edge[i].w;
 
        if (d[v] > d[u] + w) {
            printf("Ciclu negativ\n");
        }
    }
 
    printSol(V, d, p);
 
    return;
}
 
Graph *grafLab() {
    int V,E;
 
    V = 7;
    E = 14;
 
    Graph* graph = createGraph(V, E); 
 
    graph->edge[0].s = 0;
    graph->edge[0].d = 1;
    graph->edge[0].w = 3;

    graph->edge[1].s = 0;
    graph->edge[1].d = 2;
    graph->edge[1].w = 4;

    graph->edge[2].s = 0;
    graph->edge[2].d = 4;
    graph->edge[2].w = 1;

    graph->edge[3].s = 0;
    graph->edge[3].d = 5;
    graph->edge[3].w = 10;

    graph->edge[4].s = 0;
    graph->edge[4].d = 6;
    graph->edge[4].w = 3;    

    graph->edge[5].s = 1;
    graph->edge[5].d = 2;
    graph->edge[5].w = 2;   

    graph->edge[6].s = 2;
    graph->edge[6].d = 3;
    graph->edge[6].w = 2;   

    graph->edge[7].s = 2;
    graph->edge[7].d = 5;
    graph->edge[7].w = 2;   
 
    graph->edge[8].s = 4;
    graph->edge[8].d = 3;
    graph->edge[8].w = 3;   

    graph->edge[9].s = 4;
    graph->edge[9].d = 6;
    graph->edge[9].w = 1;   

    graph->edge[10].s = 5;
    graph->edge[10].d = 3;
    graph->edge[10].w = 2;  

    graph->edge[11].s = 6;
    graph->edge[11].d = 2;
    graph->edge[11].w = 1;  

    graph->edge[12].s = 6;
    graph->edge[12].d = 3;
    graph->edge[12].w = 10;  

    graph->edge[13].s = 6;
    graph->edge[13].d = 4;
    graph->edge[13].w = 2; 

    return graph;
}

int main()
{
 
    Graph *graph = grafLab();

    printf("Dijkstra\n\n");
    Dijkstra(graph);
    printf("Bellman-Ford\n\n");
    BF(graph);
 
    return 0;
}