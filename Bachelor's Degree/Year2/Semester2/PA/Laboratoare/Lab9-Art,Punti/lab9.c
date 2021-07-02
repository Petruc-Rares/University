#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct Graph {
    int sizeV;
    int **adjacencyList;
} Graph;

Graph *createGraph(int N) {
    Graph *graph = malloc(sizeof(Graph));
    graph->sizeV = N;
    graph->adjacencyList = malloc(N * sizeof(int *));

    for(int i = 0; i < N; i++) {
        graph->adjacencyList[i] = calloc(N, sizeof(int));
        for (int j = 0; j < N; j++) {
            graph->adjacencyList[i][j] = -1;
        }
    }
    return graph;
}

void addEdge(Graph *graph, int i, int j) {
    int *adjI = graph->adjacencyList[i];

    for(int k = 0; k < graph->sizeV; k++) {
        if(adjI[k] == -1) {
            adjI[k] = j;
            break;
        }
    }
    
    return;
}

Graph *grafLab() {
    Graph *graph = createGraph(10);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 9);

    addEdge(graph, 1, 0);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 8);

    addEdge(graph, 2, 1);
    addEdge(graph, 2, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 2, 7);
    
    addEdge(graph, 3, 2);
    addEdge(graph, 3, 4);
    addEdge(graph, 3, 5);

    addEdge(graph, 4, 2);
    addEdge(graph, 4, 3);

    addEdge(graph, 5, 3);
    addEdge(graph, 5, 6);

    addEdge(graph, 6, 5);
    
    addEdge(graph, 7, 2);
    addEdge(graph, 7, 8);

    addEdge(graph, 8, 1);
    addEdge(graph, 8, 7);

    addEdge(graph, 9, 0);
    return graph;
}

void printGraf(Graph *graph) {
    for (int i = 0; i < graph->sizeV; i++) {
        printf("i: %d\n", i);
        int *AdjI = graph->adjacencyList[i];

        for(int j = 0; j < graph->sizeV, AdjI[j] != -1; j++) {
            printf("i: %d -> j: %d\n", i, AdjI[j]);
        }
        printf("\n\n");
    }
}


int time;
char *colors;
int *distances;
int *parents;
int *subarbs;
int *low;
int *art;
int *punte;

void ExploreArticulatii(Graph *graph, int u) {
    distances[u] = low[u] = time++;
    // facem nodul gri
    colors[u] = 1;

    int *adjU = graph->adjacencyList[u];
    for(int i = 0; i < graph->sizeV, adjU[i] != -1; i++) {
        int crtSucc = adjU[i];

        if(colors[crtSucc] == 0) {
            parents[crtSucc] = u;
            subarbs[u]++;
            ExploreArticulatii(graph, crtSucc);
            low[u] = MIN(low[u], low[crtSucc]);
            if ((parents[u] != -1) && (low[crtSucc] >= distances[u])) {
                printf("A fost gasit punct de articulatie, caz II: %d\n", u);
                art[u] = 1;
            }
        } else {
            if(parents[u] != crtSucc) {
                low[u] = MIN(low[u], distances[crtSucc]);
            }
        }
    }
}

void Articulatii(Graph *graph) {
    printf("PUNCTE DE ARTICULATII\n");

    time = 0;
    colors = calloc(graph->sizeV, sizeof(char));
    distances = calloc(graph->sizeV, sizeof(int));
    parents = calloc(graph->sizeV, sizeof(int));
    subarbs = calloc(graph->sizeV, sizeof(int));
    art = calloc(graph->sizeV, sizeof(int));
    for (int i = 0; i < graph->sizeV; i++) {
        // initialise punte array
        art[i] = -1;
    }
    for (int i = 0; i < graph->sizeV; i++) {
        // initialise parents array
        parents[i] = -1;
    }
    low = calloc(graph->sizeV, sizeof(int));

    for (int i = 0; i < graph->sizeV; i++) {
        if (colors[i] == 0) {
            ExploreArticulatii(graph, i);
            if (subarbs[i] > 1) {
                printf("A fost gasit punct de articulatie, caz I: %d\n", i);
                art[i] = 1;
            }
        }
    }

    // acum ca totul e initializat hai sa vedem care sunt articulatiile
   /* for (int i = 0; i < graph->sizeV; i++) {
        if (art[i] != -1) {
            printf("punct de articulatie: %d\n", i);
        }
    }*/

    // hai sa vedem si d-urile si low-urile
    for (int i = 0; i < graph->sizeV; i++) {
        printf("d[%d] = %d, low[%d] = %d\n", i, i, distances[i], low[i]);
    }

    free(colors);
    free(distances);
    free(parents);
    free(low);
    free(art);
    free(subarbs);
}

void ExplorePunti(Graph *graph, int u) {
    distances[u] = low[u] = time++;
    // facem nodul gri
    colors[u] = 1;

    int *adjU = graph->adjacencyList[u];
    for(int i = 0; i < graph->sizeV, adjU[i] != -1; i++) {
        int crtSucc = adjU[i];

        if(colors[crtSucc] == 0) {
            parents[crtSucc] = u;
            ExplorePunti(graph, crtSucc);
            low[u] = MIN(low[u], low[crtSucc]);
            if (low[crtSucc] > distances[u]) punte[crtSucc] = 1;
        } else {
            if(parents[u] != crtSucc) {
                low[u] = MIN(low[u], low[crtSucc]);
            }
        }
    }
}

void Punti(Graph *graph) {
    printf("PUNTI\n");

    time = 0;
    colors = calloc(graph->sizeV, sizeof(char));
    distances = calloc(graph->sizeV, sizeof(int));
    parents = calloc(graph->sizeV, sizeof(int));
    for (int i = 0; i < graph->sizeV; i++) {
        // initialise parents array
        parents[i] = -1;
    }
    low = calloc(graph->sizeV, sizeof(int));
    punte = calloc(graph->sizeV, sizeof(int));
    for (int i = 0; i < graph->sizeV; i++) {
        // initialise punte array
        punte[i] = -1;
    }

    for (int i = 0; i < graph->sizeV; i++) {
        if (colors[i] == 0) {
            ExplorePunti(graph, i);
        }
    }

    // acum ca totul e initializat hai sa vedem care sunt puntile
    for (int i = 0; i < graph->sizeV; i++) {
        if (punte[i] != -1) {
            printf("punte intre %d si %d\n", i, parents[i]);
        }
    }

    // hai sa vedem si d-urile si low-urile
    for (int i = 0; i < graph->sizeV; i++) {
        printf("d[%d] = %d, low[%d] = %d\n", i, i, distances[i], low[i]);
    }

    free(colors);
    free(distances);
    free(parents);
    free(low);
    free(punte);
}

int main() {
    Graph *graph = grafLab();
    Articulatii(graph);
    Punti(graph);
}