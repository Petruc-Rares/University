#include <stdio.h>
#include <stdlib.h>

typedef long T;

typedef struct SortedList {
  T value;
  struct SortedList* next;
} SortedList, *SL;

// TODO : Cerința 1 & Cerința 2

void init(SortedList** list) {
  *list = NULL;
  return ;
}

int isEmpty(SortedList *list) {
  // se verifica daca lista este vida
  if (list == NULL) {
    return 1;
  } else {
    return 0;
  }
}

int contains(SortedList *list, T element) {
  SL p = list;
  while ((p != NULL) && (p->value != element)) {
    p = p -> next;
  }
  if (p == NULL) return 0;
  else return 1;
}

SortedList* allocList(T value) {
  SortedList *newElement = (SortedList*) calloc(1, sizeof(SortedList));
  newElement->value = value;
  newElement->next = NULL;
  return newElement;
}

SortedList* insert(SortedList *list, T value) {
  SortedList *p, *prev, *head;
  p = list;
  prev = head = p;
  while ((p != NULL) && (p->value < value)) {
    prev = p;
    p = p->next;
  }
  SortedList * newElement = allocList(value);
  // se verifica daca lista nu e goala (p != NULL)
  // in cazul acesta next-ul din noul element va pointa catre lista, pastrandu-se
  // o ordine crescatoare,
  if (p != NULL) {
    newElement->next = p;
    // in cazul in care elementul dorit are valoare mai mica decat oricare din
    // restul elementelor listei se modifica si headul; in caz contrar headul
    // ramane la p;
    if (p == list) {
      head = newElement;
    } else {
      prev->next = newElement;
    }
    // daca se ajunge la sfarsitul listei sau lista e goala
  } else {
    p = newElement;
    // daca lista e goala se modifica si head-ul
    if (list == NULL) {
      head = p;
    // altfel avem grija ca elementul de la finalul listei sa pointeze catre
    // noul element
    } else {
      prev->next = newElement;
    }
  }
  return head;
  /*init(&p);
  init(&prev);
  SL newElement = allocList(value);
  p = *list;
  if (isEmpty(p)) {
    p = newElement;
    *list = p;
    return ;
  } /*else {
    newElement->next = p;
    p = newElement;
    return ;
  }*/
  /* else {
    if (value < p->value) {
      newElement->next = p;
      p = newElement;
      *list = p;
      return ;
    }
    while ((p != NULL) && (p->value <= value)) {
      prev = p;
      p = p->next;
    }
    newElement->next = p;
    prev->next = newElement;
    //free(newElement);
    return ;
  }*/
}

void printList(SL lista) {
  SL p = lista;
  while (p!=NULL) {
    printf("%ld ", p->value);
    p = p->next;
  }
  printf("\n");
}

void deleteOnce(SortedList **list, T element) {
  if (contains(*list, element) == 0) {
    return ;
  } else {
    SL p = *list, prev = *list, *p1 = list, copy;
    if (p->value == element) {
      copy = *p1;
      (*list) = (*list)->next; // de ce nu merge p
      free(copy);
      return;
    }
    while ((p != NULL) && (p->value != element)) {
      prev = p;
      p = p -> next;
    }
    prev->next = p->next;
    free(p);
    return ;
  }
}

long getNth(SortedList *list, T N) {
  SL p = list;
  int k = 0;
  while (p != NULL) {
    k++;
    if (k == N) {
      return p->value;
    }
    p = p->next;
  }
  return 0;
}

void freeSortedList(SortedList **list) {
  SL* p =list, prev;
  while((*p) != NULL) {
    prev = *p;
    (*p) = (*p)->next;
    free(prev);
  }
  return ;
}

int main() {
  SL lista;
  init(&lista);
  lista = insert(lista, 0);
  printList(lista);
  lista = insert(lista, 8);
  printList(lista);
  lista = insert(lista, -3);
  printList(lista);
  insert(lista, 8);
  printList(lista);
  insert(lista, 12);
  printList(lista);
  insert(lista, 11);
  printList(lista);
  //deleteOnce(&lista, -3);
  //printList(lista);
  //freeSortedList(&lista);
  //printList(lista);
}
