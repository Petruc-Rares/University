#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char delimiter = ',';
#define MAX_SIZE 200

const char *reformat(char *string, char x) {
    // char string[MAX_SIZE] = "||||,=||,|=|,||=,==";
    int sizeStringLeft = strlen(string);
    char *token;

    int commasNo = 0;

    for (int i = 0; i < sizeStringLeft; i++) {
        if (string[i] == ',') commasNo++;
    }


    char toAdd;
    // daca avem de pus la stanga, vom pune bare verticale
    if (x == 0) {
        toAdd ='|';
    } else {
        // altfel, vom pune bare orizontale
        toAdd = '=';
    }

    int sizeStringReformat = sizeStringLeft + commasNo + 1;

    char stringReformat[MAX_SIZE];
    int j = 0;
    for (int i = 0; i < sizeStringLeft; i++) {
        if (string[i] != ',') {
            stringReformat[j] = string[i];
        } else {
            stringReformat[j] = toAdd;
            stringReformat[j+1] = ',';
            j++;
        }
        j++;
    }
    stringReformat[j] = toAdd;

    stringReformat[sizeStringReformat] = '\0';

    const char *string2 = malloc(MAX_SIZE);
    strcpy(string2, stringReformat);

    return string2;
}

void kGarduri() {
    int n = 6;
    int k = 2;

    int *D = (int *) malloc(sizeof(int) * (n + 1));
    char graphic[n+1][MAX_SIZE];

    D[0] = 1;
    strcpy(graphic[0], "\0");
    D[1] = 1;
    strcpy(graphic[1], "|\0");

    for (int i = 2; i <= n; i++) {
        D[i] = D[i-1] + D[i-k];

        const char *stringLeft = reformat(graphic[i-1], 0);

        const char *stringRight = reformat(graphic[i-k], 1);
        char stringRightSeparated[MAX_SIZE]; 
        stringRightSeparated[0] = delimiter;
        strcpy(stringRightSeparated + 1, stringRight);

        strcpy(graphic[i], strcat(stringLeft, stringRightSeparated));
        printf("graphic[%d] = %s\n\n\n", i, graphic[i]);
    }

    printf("Numarul de solutii posibile este egal cu: %d\n", D[n]);

    free(D);
}

void subsetModulo(int arr[], int l, int r, int sum, char *elements){
    char str[MAX_SIZE];

    if (l > r) {
        if ((sum % 3 == 0) && (strcmp(elements, "") != 0)) {
            printf("elementele %s dau suma %d, ce este divizibila cu 3\n", elements, sum);
        }
        return;
    }
 
    sprintf(str, "%d", arr[l]);
    char *string2 = malloc(MAX_SIZE);
    strcpy(string2, str);


    char *string1 = malloc(MAX_SIZE);
    strcpy(string1, elements);
    strcat(string1, " ");
    strcat(string1, string2);

    char *string1a = malloc(MAX_SIZE);
    strcpy(string1a, elements);

    subsetModulo(arr, l+1, r, sum+arr[l], string1);

    subsetModulo(arr, l+1, r, sum, string1a);

    free(string1);
    free(string1a);
    free(string2);
}

void cateSume() {
    int v[] = {3, 1, 2, 4};

    int N = sizeof(v)/sizeof(v[0]);

    int D[N + 1][3];

    D[0][0] = D[0][1] = D[0][2] = 0;

    for (int i = 1; i <= N; i++) {
        if (v[i-1] % 3 == 0) {
            D[i][0] = 1 + 2*D[i-1][0];
            D[i][1] = 2 * D[i-1][1];
            D[i][2] = 2 * D[i-1][2];
        } else if (v[i-1] % 3 == 1) {
            D[i][0] = D[i-1][0] + D[i-1][2];
            D[i][1] = 1 + D[i-1][1] + D[i-1][0];
            D[i][2] = D[i-1][2] + D[i-1][1];
        } else if (v[i-1] % 3 == 2) {
            D[i][0] = D[i-1][0] + D[i-1][1];
            D[i][1] = D[i-1][1] + D[i-1][2];
            D[i][2] = 1 + D[i-1][2] + D[i-1][0];
        }
    }

    for (int i = 0; i <= N; i++) {
        for (int j=0; j < 3; j++) {
            printf("%d ", D[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");

    printf("Numarul de sume care se pot forma este egal cu: %d\n", D[N][0]);

    char *string3 = calloc(1, sizeof(char));
    subsetModulo(v, 0, 3, 0, string3);
}

int main() {
    kGarduri();
    cateSume();
}