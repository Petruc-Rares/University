#include <stdio.h>
#include <limits.h>

void towerOfHanoi(int n, char x, char y, char z) {
    if (n > 0) {
        towerOfHanoi(n - 1, x, z, y);
        printf("discul cu numarul: %d se muta de la %c  %c\n", n, x, y);
        towerOfHanoi(n - 1, z, y, x);
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int findMax(int *v, int start, int end) {

    int headsDifference = end - start;
    // cazul daca ramane un unic element
    // il returnam pe el insusi
    if (headsDifference == 0) return v[end];
    // cazul daca raman doua elemente
    // le comparam si returnam elementul mai mare
    else if (headsDifference == 1) {
        return max(v[start], v[end]);
    }

    int mid = (end + start) / 2;

    return max(findMax(v, start, mid), findMax(v, mid + 1, end));
}

void merge(int *A, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;

    int S[n1 + 1];
    int D[n2 + 1];
    
    int i = 0;
    int j = 0;

    for (i = 0; i < n1; i++) {
        S[i] = A[p + i];
    }

    for (j = 0; j < n2; j++) {
        D[j] = A[q + j + 1];
    }

    S[n1] = INT_MAX;
    D[n2] = INT_MAX;

    i = 0;
    j = 0;

    for (int k = p; k <= r; k++) {
        if (S[i] < D[j]) {
            A[k] = S[i];
            i++;
        }
        else {
            A[k] = D[j];
            j++;
        }
    }
    return ;
}

void mergeSort(int *a, int start, int end) {

    if (start < end) {
        int mid = (start + end) / 2;

        mergeSort(a, start, mid);
        mergeSort(a, mid + 1, end);
        merge(a, start, mid, end);
    }
    return ;
}

int binarySearch(int *v, int left, int right, int key) {
    if (left > right) {
        return -1;
    }
    int mid = (left + right) / 2;

    if (v[mid] == key) {
        return mid;
    } else {
        if (key < v[mid]) {
            return binarySearch(v, left, mid - 1, key);
        } else {
            return binarySearch(v, mid + 1, right, key);
        }
    }
}

int getCounts(int *v, int l, int r, int key) {
    if (l == r) {
        if (v[l] == key) return 1;
        return 0;
    }
    
    int mid = (l + r) / 2;

    return getCounts(v, l, mid, key) + getCounts(v, mid + 1, r, key);

}

int main() {
    /* Turnurile din Hanoi */
    int numberDisks = 2;
    towerOfHanoi(numberDisks, 'A', 'B', 'C');

    printf("\n\n");

    /* Elementul maxim al unui vector */
    int v[] = {3, 2, 5, 2, 4};
    int sizeV = sizeof(v)/sizeof(v[0]);

    int maxNumber = findMax(v, 0, sizeV - 1); 
    printf("Elementul maxim din v este: %d\n", maxNumber);

    int a[] = {3, 2, 1, 4, 3, 5, 2};
    int sizeA = sizeof(a)/sizeof(a[0]);

    printf("\n\n");

    /* Sortare prin interclasare */

    printf("Vectorul inainte de sortare:\n");

    for (int i = 0; i < sizeA; i++) {
        printf("%d ", a[i]);
    }

    printf("\n\n");

    mergeSort(a, 0, sizeA - 1);

    printf("Vectorul dupa sortare:\n");

        for (int i = 0; i < sizeA; i++) {
        printf("%d ", a[i]);
    }

    printf("\n\n");


    /* Cautare binara */
    int array[] = {1, 3, 4, 5, 7, 8, 9};
    int sizeArray = sizeof(array) / sizeof(array[0]);
    
    int x = 3;

    int index = binarySearch(array, 0, sizeArray - 1, x);
    printf("elementul: %d se gaseste in array la pozitia %d\n", x, index);

    printf("\n\n");

    /* Numarul de aparitii ale unui element intrs-un vector nesortat */
    int vector[] = {2, 1, 3, 1, 1, 4, 2, 1, 8, 1};
    int sizeVector = sizeof(vector)/sizeof(vector[0]);
    int key = 1;

    int noAparitii = getCounts(vector, 0, sizeVector - 1, key);
    printf("numarul de aparitii ale elementului %d in vector este egal cu: %d\n", key, noAparitii);

    return 0;
}