#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct productInfo {
    float raport;
    int index;
};

struct showInterval {
    int start;
    int end;
};

int compareDescendingFloat(const void *a, const void *b) {

  const struct productInfo *x = a;
  const struct productInfo *y = b;

  return y->raport - x->raport;
}

int compareAscendingInt(const void *a, const void *b) {

  const struct showInterval *x = a;
  const struct showInterval *y = b;

  return x->end - y->end;
}

int main() {
    /* exercitiul 2 */
    int M = 50;
    int v[] = {60, 120, 100};
    int m[] = {10, 30, 20};

    int noProducts = sizeof(v)/sizeof(v[0]);
    float raport[noProducts];

    for (int it = 0; it < noProducts; it++) {
        raport[it] = (float) v[it] / m[it];
    }
    
    struct productInfo productsInfo[noProducts];
    for (int it = 0; it < noProducts; it++) {
        productsInfo[it].raport = raport[it];
        productsInfo[it].index = it;
    }

    // sortam ca sa avem produsele in ordine dupa valoare per kg
    qsort(productsInfo, noProducts, sizeof(struct productInfo), compareDescendingFloat);

    float totalValue = 0;
    int copyM = M;

    for (int it = 0; it < noProducts; it++) {
        int crtIndex = productsInfo[it].index;

        // daca produsul incape in totalitate
        if (m[crtIndex] < copyM) {
            totalValue += v[crtIndex];
            printf("Se adauga greutatea %d cu valoarea de %d din obiectul %d in totalitate\n", m[crtIndex], v[crtIndex], crtIndex);
            copyM -= m[crtIndex];
        } else {
            // daca incape doar o parte din produs
            printf("Se adauga greutatea %d cu valoarea de %.2f din obiectul %d in proportie de %.2f%%\n", copyM, copyM * (float) v[crtIndex]/m[crtIndex], crtIndex, (float) copyM/m[crtIndex] * 100);
            totalValue += copyM * (float) v[crtIndex]/m[crtIndex];
            break;
        }
    }

    printf("valoare totala rucsac = %0.2f\n", totalValue);

    printf("\n\n");

    /* exercitiul 3 */
    int s[] = {10, 11, 12, 14, 16, 17};
    int t[] = {11, 13, 13, 18, 17, 19};

    int noShows = sizeof(s)/sizeof(s[0]);

    struct showInterval showsInterval[noShows];

    // copiem timpii intr-un vector de structuri
    // ce contin intervalele pentru fiecare spectacol
    for (int it = 0; it < noShows; it++) {
        showsInterval[it].start = s[it];
        showsInterval[it].end = t[it];
    }

    qsort(showsInterval, noShows, sizeof(struct showInterval), compareAscendingInt);

    int crtEnd = INT_MIN;
    int noPlannedShows = 0;

    for (int it = 0; it < noShows; it++) {
        if (showsInterval[it].start >= crtEnd) {
            printf("interval spectacol %d: start: %d end: %d\n", noPlannedShows, showsInterval[it].start, showsInterval[it].end);
            crtEnd = showsInterval[it].end;
            noPlannedShows++;
        }
    }

    printf("Au fost planificate %d spectacole.\n", noPlannedShows);

    return 0;
}