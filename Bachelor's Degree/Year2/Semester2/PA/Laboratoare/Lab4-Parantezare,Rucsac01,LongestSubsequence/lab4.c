#include <stdio.h>

int max(int a, int b) {
    if (a >= b) {
        return a;
    }
    return b;
}

void printUpperTriangle(int n, int matrix[n][n]) {
    for (int k = n - 1; k >= 0; k--) {
        for (int i = 0; i < n - k; i++) {
            int j = i + k;
            //printf("(%d, %d)\n", i, j);
            for (int aux = 0; aux < (k+3) * 2; aux++) printf(" ");
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int sNoRows;

void printParantezare(int S[sNoRows][sNoRows], int i, int j) {
    if (i == j) {
        printf("Matricea %d", i);
        return;
    }
    int k = S[i][j];

    printf("(");
    printParantezare(S, i, k - 1);
    printf(") * (");
    printParantezare(S, k, j);
    printf(")");
}

void parantezare() {
    int p[] = {2, 4, 3, 5, 2};
    int noMatrixes = sizeof(p)/sizeof(p[0]) - 1;

    sNoRows = noMatrixes;

    int M[noMatrixes][noMatrixes];
    int S[noMatrixes][noMatrixes];

    for (int i = 0; i < noMatrixes; i++) {
        M[i][i] = 0;
        S[i][i] = 0;
    }    

    for (int l = 1; l <= noMatrixes - 1; l++) {
        for (int i = 0; i < noMatrixes - l; i++) {
            int j = i + l;
            M[i][j] = __INT_MAX__;
            
            for (int k = i; k < j; k++) {
                int q = M[i][k] + M[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                if (q < M[i][j]) {
                    M[i][j] = q;
                    S[i][j] = k+1;
                }
            }
        }
    }


    printf("Triunghiul m:\n");

    printUpperTriangle(noMatrixes, M);

    printf("Triunghiul s:\n");
    printUpperTriangle(noMatrixes, S);


    printf("\n\n");

    printf("Numarul minim de operatii este: %d\n", M[0][noMatrixes - 1]);

    printf("parantezare: \n");
    printParantezare(S, 0, noMatrixes - 1);
    printf("\n\n");
    return ;
}

void backback_0_1() {
    int totalWeight = 50;
    int m[] = {10, 30, 20};
    int v[] = {60, 120, 100};

    int noObjects = sizeof(m) / sizeof(m[0]);

    int D[noObjects + 1][totalWeight + 1];


    // obiectul 0 nu exista, e folosita
    // doar pe post de santinela linia corespunzatoare lui
    // iar intr-un rucsac cu 0kg nu intra nimic, deci avem valoarea 0
    for (int j = 0; j <= totalWeight; j++) {
        D[0][j] = 0;
    }

    for (int i = 0; i <= noObjects; i++) {
            D[i][0] = 0;
    }
           

    for (int i = 1; i <= noObjects; i++) {
        for (int j = 10; j <= totalWeight; j += 10) {
            if (j >= m[i - 1]) {
                // se ia maximul dintre obiectul i-1 impreuna cu ce mai ramane loc in ghiozdan
                // si valoarea obtinuta la un pas anterior
                D[i][j] = max(D[i - 1][j], v[i - 1] + D[i - 1][j - m[i - 1]]);
            } else {
                // daca nu e loc de noul obiect in ghiozdan
                // se ia valoarea obtinuta la un pas anterior
                D[i][j] = D[i - 1][j];
            }
        }
    }

    printf("Valoarea maxima ce intra in ghiozdan este: %d\n\n", D[noObjects][totalWeight]);

    printf("Tabelul cu valori in functie de greutatea ghiozdanului este:\n");
    for (int i = 0; i <= noObjects; i++) {
        for (int j = 0; j <= totalWeight; j+= 10) {
            printf("%d ", D[i][j]);   
        }
        printf("\n");
    }

    printf("\n\n");
    

    int copyWeight = totalWeight;
    int copyValue = D[noObjects][totalWeight];

    for (int i = noObjects; i > 0; i--) {
          
        // daca rucascul are aceeasi valoare si pentru obiectul curent
        // si pentru cel precedent, sarim peste, deoarece inseamna ca
        // obiectul curent nu a fost inclus in rucsac
        if (copyValue == D[i - 1][copyWeight]) 
            continue;        
        else {
            printf("in rucsac este continut obiectul %d cu masa: %d si valorea: %d\n", i - 1,  m[i - 1], v[i - 1]);
              
            copyValue = copyValue - v[i - 1];
            copyWeight = copyWeight - m[i - 1];
        }

        if (copyWeight == 0) break;
    }

    printf("\n\n");
    return ;
}

void longest_subsequence() {
    int v[] = {1, 2, 9, 3, 8, 4, 7};
    int noElements = sizeof(v)/sizeof(v[0]);
    int d[noElements];

    // initializam lungimile subsirurilor la 1
    for (int i = 0; i < noElements; i++) {
        d[i] = 1;
    }

    for (int i = 0; i < noElements; i++) {
        int crtElem = v[i];

        for (int j = i - 1; j >= 0; j--) {
            if (v[j] < crtElem) {
                d[i] = max(d[i], d[j] + 1);
            }
        }   
    }

    int longestSubsequence = -1;
    int biggestNumberInSequence = v[0];
    int posBiggestNumber = 0;

    for (int i = 0; i < noElements; i++) {
        if (d[i] > longestSubsequence) {
            longestSubsequence = d[i];
            biggestNumberInSequence = v[i];
            posBiggestNumber = i;
        }
    }

    printf("Cel mai lung subsir crescator are lungimea: %d\n", longestSubsequence);

    printf("Sirul corespunzator lungimii maxime este:\n");
    for (int i = 0; i <= posBiggestNumber; i++) {
        if (v[i] <= biggestNumberInSequence) {
            printf("%d ", v[i]);
        }
    }

    printf("\n\n");


    return ;
}

int main() {
    /* problema 1 */
    parantezare();

    /* problema 2 */    
    backback_0_1();

    /* problema 3 */
    longest_subsequence();
}