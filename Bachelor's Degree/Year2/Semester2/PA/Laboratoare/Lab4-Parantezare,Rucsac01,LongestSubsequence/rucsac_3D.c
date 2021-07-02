#include <stdio.h>

int max(int a, int b) {
    if (a >= b) {
        return a;
    }
    return b;
}

void backback_0_1() {
    int B = 13;
    int L = 3;
    int crtPrice[] = {9, 8, 7, 8, 6, 9, 5, 2, 6};
    int maxLoss[] = {1, 1, 2, 2, 1, 1, 1, 1, 2};
    int maxProfit[] = {1, 1, 2, 3, 2, 3, 4, 2, 1};

    int N = sizeof(maxLoss) / sizeof(maxLoss[0]);

    int D[N + 1][B + 1][L + 1];


    // obiectul 0 nu exista, e folosita
    // doar pe post de santinela linia corespunzatoare lui
    // iar intr-un rucsac cu 0kg nu intra nimic, deci avem valoarea 0
    for (int j = 0; j <= B; j++) {
        for (int k = 0; k <= L; k++)
        D[0][j][k] = 0;
    }      

    for (int i = 1; i <= N; i++) {
        for (int j = 0; j <= B; j++) {
            for (int k = 0; k <= L; k++) {
                if ((j >= crtPrice[i - 1]) && (k >= maxLoss[i-1])) {
                    // se ia maximul dintre obiectul i-1 impreuna cu ce mai ramane loc in ghiozdan
                    // si valoarea obtinuta la un pas anterior
                    D[i][j][k] = max(D[i - 1][j][k], maxProfit[i - 1] + D[i - 1][j - crtPrice[i - 1]][k-maxLoss[i-1]]);
                } else {
                    // daca nu e loc de noul obiect in ghiozdan
                    // se ia valoarea obtinuta la un pas anterior
                    D[i][j][k] = D[i - 1][j][k];
                }
            }
        }
    }

    printf("Profitul maxim este este: %d\n\n", D[N][B][L]);

/*
    printf("Tabelul cu valori in functie de greutatea ghiozdanului este:\n");
    for (int i = 0; i <= noObjects; i++) {
        for (int j = 0; j <= totalWeight; j+= 10) {
            printf("%d ", D[i][j]);   
        }
        printf("\n");
    }

    printf("\n\n");
*/
}

int main() {
    backback_0_1();
}