#include <stdio.h>

void graphColoring(int graph[4][4], int crtNode, int colors[3], int noColors) {
    if (crtNode == 4) {
        for (int i = 0; i < 4; i++) {
            printf("nodul %d are culoarea %d\n", i, colors[i]);
        }
        printf("\n\n");
        return;
    }

    char flag = 0;

    for (int i = 0; i < noColors; i++) {
        colors[crtNode] = i;
        flag = 0;

        // verific daca am conflict
        for (int j = 0; j < crtNode; j++) {
            if (graph[crtNode][j] != 0) {
                if (colors[crtNode] == colors[j]) {
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 1) continue;
        graphColoring(graph, crtNode + 1, colors, noColors);
    }
}

int main() {
    int graph[4][4] = {
        { 0, 1, 1, 0},
        { 1, 0, 1, 1},
        { 1, 1, 0, 1},
        { 0, 1, 1, 0},
    };
    int colors[4];
    int noColors = 3;

    graphColoring(graph, 0, colors, noColors);
}