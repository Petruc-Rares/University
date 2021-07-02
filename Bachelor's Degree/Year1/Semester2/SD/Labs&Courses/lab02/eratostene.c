#include <stdio.h>

typedef int T;
#include "SortedList.h"

SortedList* getNaturals(int A, int B) {
  SortedList* nats;
  int i;
  init(&nats);
  for (i = A; i <= B; i++) {
    insert(&nats, i);
  }
  return nats;
}


SortedList* getPrimes(int N) {
  SortedList* nats = getNaturals(2, N);
	SortedList *aux1, *aux2, *prev;
	aux1 = nats;
  /*
	while (!isEmpty(aux1)) {
	  aux2 = aux1;
	  while (!isEmpty(aux2->next)) {
	    if (aux2->next->value % aux1->value == 0){
	      aux3 = aux2->next;
	      aux2->next = aux3->next;
	      free(aux3);
	    } else {
	      aux2 = aux2->next;
	    } 
	  }
	  aux1 = aux1->next;
	}*/

  while (!isEmpty(aux1)) {

    aux2 = aux1->next;
    prev = aux1;

    while (!isEmpty(aux2)) {

      if (aux2->value % aux1->value == 0) {

        prev->next = aux2->next;
        free(aux2);

      }

      prev = aux2;
      aux2 = aux2->next;

    }

    aux1 = aux1->next;
  }
	return nats;
  /*SortedList *p = nats, *p1 = nats;
  // TODO: Cerința 4
  long k = 2, auxk =  k;
  while (isEmpty(p1) == 0) {
    p1 = p;
    p1 ->next = p->next;
    while (auxk <= N) {
      pointForward(p1);
      auxk += auxk;
    }
    auxk = k + 1;
  }
  return nats;*/
  /*SortedList* aux1, *aux2, *aux3;
  aux1 = nats;
  while(!isEmpty(aux1)) {
    aux2 = aux1;
    while(!isEmpty(aux2->next)) {
      if (aux2->next->value % aux1->value == 0) {
        aux3 = aux2->next;
        aux2->next = aux3->next;
        free(aux3);
      } else {
        aux2 = aux2->next;
      }
    }
    aux1 = aux1->next;
  }
  return nats;*/
  /*SortedList aux1, aux2, aux3*/
}

void printInts(SortedList* list) {
  while (!isEmpty(list)) {
    printf("%d ", list->value);
    list = list->next;
  }
  printf("\n");
}

int main(int argc, char* argv[]) {
  printInts(getPrimes(100));
  return 0;
}
