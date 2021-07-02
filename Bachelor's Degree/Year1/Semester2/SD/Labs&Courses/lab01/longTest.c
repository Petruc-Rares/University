#include <stdio.h>

typedef long T;

#include "OrderedSet.h"


OrderedSet* construiesteMultimea(long nr, long margInf, long margSup) {
  long intra, multiplu, k = 0;
  OrderedSet *multiplii = createOrderedSet((margSup - margInf) / nr + 1);
  // verificam de cate ori intra nr in margInf
  intra = margInf / nr;
  if (intra * nr == margInf) {
    multiplu = margInf;
  } else {
    multiplu = intra * nr + nr;
  }
  while (multiplu <= margSup) {
    multiplii->elements[k++] = multiplu;
    multiplii->size++;
    multiplu = multiplu + nr;
  }
  return multiplii;
}

void printMultime(OrderedSet* multiplii) {
  long i = 0;
  printf("There are %lu elements: ", multiplii->size);
  for (i = 0; i < multiplii->size; i++) {
    printf("%ld ", multiplii->elements[i]);
  }
  printf("\n");
  return ;
}

int main() {
  OrderedSet *s1, *s2, *sU, *sI;
  s1 = construiesteMultimea(3, 4, 25);
  s2 = construiesteMultimea(4, 5, 30);
  sU = unionOrderedSets(s1, s2);
  sI = intersectOrderedSets(s1, s2);
  printMultime(s1);
  printMultime(s2);
  printMultime(sU);
  printMultime(sI);
}
