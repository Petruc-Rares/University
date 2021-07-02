#include <stdio.h>
#include <stdlib.h>

void printPermutari(int crtArray[], int crtSize, int array[], int size) {
    if (crtSize == size) {
        printf("{");
        for (int i = 0; i < size - 1; i++) {
            printf("%d, ", crtArray[i]);
        }
        printf("%d", crtArray[size-1]);
        printf("} ");
        return;
    }
    int flag = 0;

    for (int i = 0; i < size; i++) {
        flag = 0;
        crtArray[crtSize] = array[i];
        
        // check for conflicts
        for (int j = 0; j < crtSize; j++) {
            if (crtArray[crtSize] == crtArray[j]) {
                flag = 1;
            }
        }
        if(flag == 1) continue;

        // no conflicts found
        printPermutari(crtArray, crtSize + 1, array, size);
    }
}

void permutari() {
    int v[] = {1, 2, 3};
    int sizeV = sizeof(v)/sizeof(v[0]);
    int aux[sizeV];
    printPermutari(aux, 0, v, sizeV);

    printf("\n\n");
}

void printSubmultimi(int v[], int sizeV, int crtIdx, int marked[]) {
    if (crtIdx == sizeV) {
        
        printf("{");
        for (int i = 0; i < crtIdx - 1; i++) {
            if (marked[i] == 1) {
                printf("%d ", v[i]);
            }
        }
        if (marked[sizeV - 1] == 1) {
            printf("%d", v[sizeV - 1]);
        }
        printf("} ");
        return;
    }

    marked[crtIdx] = 0;
    printSubmultimi(v, sizeV, crtIdx + 1, marked);
    marked[crtIdx] = 1;
    printSubmultimi(v, sizeV, crtIdx + 1, marked);

}

void submultimi() {
    int v[] = {1, 2, 3};
    int sizeV = sizeof(v)/sizeof(v[0]);
    int marked[sizeV];
    printSubmultimi(v, sizeV, 0, marked);

    printf("\n\n");
}

#define size 4

void printDame(int matrix[][size], int N, int crtColumn, int v[]) {
    if (crtColumn == N) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (v[j] == i) {
                    printf("1 ");
                } else {
                    printf("0 ");
                }
            }
            printf("\n");
        }
        printf("{");
        for (int j = 0; j < N; j++) {
            printf("D%d: ", j);
            printf("(");
            printf("%d, %d", j, v[j]);
            printf(") ");
        }
        printf("}");

        printf("\n\n");
        return;
    }

    char flag = 0;

    for (int crtRow = 0; crtRow < N; crtRow++) {
        flag = 0;
        v[crtColumn] = crtRow;

        // check for conflicts
        for (int i = 0; i < crtColumn; i++) {
            if ((v[crtColumn] == v[i]) || (abs((v[crtColumn] - v[i])/(crtColumn - i)) == 1)) {
                flag = 1;
                break;
            }
        }
        if (flag == 1) continue;
        printDame(matrix, N, crtColumn + 1, v);
    }
}

void dame() {
    int N = 4;
    int matrix[N][N];
    int v[N];
    for (int i = 0; i < N; i++) {
        v[i] = -1;
    }

    printDame(matrix, N, 0, v); 
}

int main() {
    permutari();
    submultimi();
    dame();
}