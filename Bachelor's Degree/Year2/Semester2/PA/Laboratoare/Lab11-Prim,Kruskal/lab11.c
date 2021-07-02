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

Graph *grafLab() {
    int V,E;
 
    V = 12;
    E = 30;
 
    Graph* graph = createGraph(V, E); 
 
    // noduri din A
    graph->edge[0].s = 0;
    graph->edge[0].d = 1;
    graph->edge[0].w = 9;

    graph->edge[1].s = 0;
    graph->edge[1].d = 6;
    graph->edge[1].w = 10;

    graph->edge[2].s = 0;
    graph->edge[2].d = 7;
    graph->edge[2].w = 6;

    graph->edge[3].s = 0;
    graph->edge[3].d = 8;
    graph->edge[3].w = 3;

    // noduri din B
    graph->edge[4].s = 1;
    graph->edge[4].d = 0;
    graph->edge[4].w = 9;   

    graph->edge[5].s = 1;
    graph->edge[5].d = 6;
    graph->edge[5].w = 11;    

    graph->edge[6].s = 1;
    graph->edge[6].d = 2;
    graph->edge[6].w = 5;   

    // noduri din C
    graph->edge[7].s = 2;
    graph->edge[7].d = 1;
    graph->edge[7].w = 5;   

    graph->edge[8].s = 2;
    graph->edge[8].d = 3;
    graph->edge[8].w = 8;   

    graph->edge[9].s = 2;
    graph->edge[9].d = 4;
    graph->edge[9].w = 12;   
 
    // noduri din D
    graph->edge[10].s = 3;
    graph->edge[10].d = 2;
    graph->edge[10].w = 8;   

    graph->edge[11].s = 3;
    graph->edge[11].d = 5;
    graph->edge[11].w = 9;   

    // noduri din E
    graph->edge[12].s = 4;
    graph->edge[12].d = 2;
    graph->edge[12].w = 12;  

    graph->edge[13].s = 4;
    graph->edge[13].d = 5;
    graph->edge[13].w = 2;  

    // noduri din F
    graph->edge[14].s = 5;
    graph->edge[14].d = 3;
    graph->edge[14].w = 9;  

    graph->edge[15].s = 5;
    graph->edge[15].d = 4;
    graph->edge[15].w = 2; 

    // noduri din G
    graph->edge[16].s = 6;
    graph->edge[16].d = 0;
    graph->edge[16].w = 10;  

    graph->edge[17].s = 6;
    graph->edge[17].d = 1;
    graph->edge[17].w = 11; 

    graph->edge[18].s = 6;
    graph->edge[18].d = 7;
    graph->edge[18].w = 4; 

    // noduri din H
    graph->edge[19].s = 7;
    graph->edge[19].d = 0;
    graph->edge[19].w = 6;  

    graph->edge[20].s = 7;
    graph->edge[20].d = 6;
    graph->edge[20].w = 4; 

    // noduri din I
    graph->edge[21].s = 8;
    graph->edge[21].d = 0;
    graph->edge[21].w = 3; 

    graph->edge[22].s = 8;
    graph->edge[22].d = 9;
    graph->edge[22].w = 5; 

    graph->edge[23].s = 8;
    graph->edge[23].d = 11;
    graph->edge[23].w = 8; 

    // noduri din J
    graph->edge[24].s = 9;
    graph->edge[24].d = 8;
    graph->edge[24].w = 5; 

    graph->edge[25].s = 9;
    graph->edge[25].d = 10;
    graph->edge[25].w = 14; 

    // noduri din K
    graph->edge[26].s = 10;
    graph->edge[26].d = 9;
    graph->edge[26].w = 14; 

    graph->edge[27].s = 10;
    graph->edge[27].d = 11;
    graph->edge[27].w = 7; 

    // noduri din L
    graph->edge[28].s = 11;
    graph->edge[28].d = 8;
    graph->edge[28].w = 8; 

    graph->edge[29].s = 11;
    graph->edge[29].d = 10;
    graph->edge[29].w = 7; 

    return graph;
}

void printGraph(Graph *graph) {
    int E = graph->E;
    for (int i = 0; i < E; i++) {
        printf("%c %c %d\n", (char)(graph->edge[i].s + 65), (char) (graph->edge[i].d + 65), graph->edge[i].w);
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

void printPrim(int V, int *parent) {
    printf("{");
    for (int i = 0; i < V; i++) {
        if (parent[i] != -1) {
            printf("(%c, %c), ", (char)(i + 65), (char)(parent[i] + 65));
        }
    }
    printf("\b\b");
    printf("}\n");
}

void Prim(Graph *graph) {
    int E = graph->E;
    int V = graph->V;
    Edge *result = malloc(V * sizeof(Edge));

    int *d = calloc(V, sizeof(int));
    int *visited = calloc(V, sizeof(int));
    int *p = calloc(E, sizeof(int));
    for (int i = 0; i < V; i++) {
        d[i] = INT_MAX;
        p[i] = -1;
    }

    d[8] = 0;
    int crtResultIt = 0;

    for (int i = 0; i < V; i++) {
        int j;

        int nextNode;
        int minDistance = INT_MAX;
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

            if ((visited[v] == 0) && (d[v] > w)) {
                d[v] = w;
                p[v] = u;
            }
        }
    }

    printPrim(graph->V, p);
}

typedef struct Arbore {
    int root;
    int size;
} Arbore;

int comp(const void* a, const void* b)
{
    Edge* a1 = (Edge*)a;
    Edge* b1 = (Edge*)b;
    return a1->w > b1->w;
}


// C program for Kruskal's algorithm to find Minimum
// Spanning Tree of a given connected, undirected and
// weighted graph
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 

int getRoot(int idx, Arbore *arb) {
    if (arb[idx].root != idx) {
        arb[idx].root = getRoot(arb[idx].root, arb);
    }

    return arb[idx].root;
}

void make_union(Arbore *arbs, int x, int y) {
    int x_root = getRoot(x, arbs);
    int y_root = getRoot(y, arbs);
 
    if (arbs[x_root].size < arbs[y_root].size) {
        arbs[x_root].root = y_root;
        arbs[y_root].size += arbs[x_root].size;
        arbs[x_root].size = 0;
    } else if (arbs[x_root].size > arbs[y_root].size) {
        arbs[y_root].root = x_root;
        arbs[x_root].size += arbs[y_root].size;
        arbs[y_root].size = 0;
    } else {
        // case equality
        arbs[y_root].root = x_root;
        arbs[x_root].size += arbs[y_root].size;
        arbs[y_root].size = 0;
    }
}

void Kruskal(Graph *graph) {
    int E = graph->E;
    int V = graph->V;
    Edge* A = calloc(E, sizeof(Edge));
    Arbore *Arb = calloc(V, sizeof(Arbore));

    for (int i = 0; i < V; i++) {
        Arb[i].root = i;
        Arb[i].size = 1;
    }

    int **visited = calloc(E, sizeof(int *));
    for (int i = 0; i < E; i++) {
        visited[i] = calloc(E, sizeof(int));
    }
    // sorteaza_asc(E, w)
    qsort(graph->edge, E, sizeof(Edge), comp);

    int j = 0;
    int size_arb = 0;
    for (int i = 0; i < E, j < E; i++) {
        Edge u_v = graph->edge[j++];
        if ((visited[u_v.s][u_v.d] == 1) || (visited[u_v.d][u_v.s] == 1)) {
            i--;
            continue;
        }

        visited[u_v.d][u_v.s] = 1;
        visited[u_v.s][u_v.d] = 1;
    
        int root_u = getRoot(u_v.s, Arb);
        int root_v = getRoot(u_v.d, Arb);  

        if (root_u != root_v) {
            A[i] = u_v;
            size_arb++;
            make_union(Arb, root_u, root_v);
        }
    }


    printf("{");
    for (int i = 0; i < size_arb; i++) {
        Edge u_v = A[i];
        printf("(%c, %c), ", (char)(u_v.d+65), (char)(u_v.s+65));
    }
    printf("\b\b");
    printf("}\n");
    
}

int main() {
    Prim(grafLab());
    Kruskal(grafLab());
}