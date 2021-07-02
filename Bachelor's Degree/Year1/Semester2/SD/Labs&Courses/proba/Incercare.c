#include <stdio.h>

void functie(int *a) {
  int b = *a;
  printf("%d %d\n", b, *a);
  b = 3;
  printf("%d %d\n", b, *a);
}

int main() {
  int a = 2;
  functie(&a);
}
