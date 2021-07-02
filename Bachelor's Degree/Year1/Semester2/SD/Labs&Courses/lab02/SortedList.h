#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------

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

  // sau return (list == NULL)
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

void insert(SortedList **list, T value) {
  SortedList *p, *prev, **newlist;
  //init(&p);
  //init(&prev);
  SL newElement = allocList(value);
  p = *list;
  if (isEmpty(p)) {
    *list = newElement; // de ce nu merge daca in loc de *list pun p?
    return ;
  } /*else {
    newElement->next = p;
    p = newElement;
    return ;
  }*/
   else {
    if (value < p->value) {
      newElement->next = p;
      *list = newElement; // de ce nu merge daca in loc de *list pun p?
      return ;
    }
    while ((p != NULL) && (p->value <= value)) {
      prev = p;
      p = p->next;
    }
    newElement->next = p;
    prev->next = newElement;
    return ;
  }
  /* SortedList *head = *list;
     while(head) {
	 if ((*list) = NULL || *list->value > value) {
      newElement = allocLIst(value);
      (*list) = newElement;
     } else {
       insert(&(list->next), value);
     }
    }
  */
}

void deleteOnce(SortedList **list, T element) {
  if (contains(*list, element) == 0) {
    return ;
  } else {
    SL p = *list, prev = *list, *p1 = list, copy;
    if (p->value == element) {
      copy = *p1;
      (*p1) = (*p1)->next; // de ce nu merge p
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

long length(SortedList *list) {
  SL p;
  T k = 0;
  p = list;
  while (p) {
    p = p->next;
    k++;
  }
  return k;
}

long getNth(SortedList *list, T N) {
  /*SL p = list;
  int k = 0;
  while (p != NULL) {
    k++;
    if (k == N) {
      return p->value;
    }
    p = p->next;
  }
  return 0;*/

	// implementarea mea
	SL p = list;
	N--;
	while ((p != NULL) && (N != 0)) {

		p = p->next;
		N--;

	}
	if(N != 0) return 0;
	return p->value;
}

void freeSortedList(SortedList **list) {
  SL *p = list, prev;
  /*while((*p) != NULL) {
    prev = *p;
    (*p) = (*p)->next;
    free(prev);
  }*/
  while ((*list) != NULL) {
    prev = *list;
    (*list) = (*list)->next;
    free(prev);
  }
  return ;
}

#endif
