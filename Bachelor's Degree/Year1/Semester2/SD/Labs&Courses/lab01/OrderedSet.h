#ifndef ORDERED_SET_H
#define ORDERED_SET_H

#include <stdio.h>                                          // needed for printf
#include <stdlib.h>                           // needed for alloc, realloc, free

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------

/*
  OrderedSet represents a set with elements of type T stored in memory
  in an ordered fashion.
*/

typedef struct OrderedSet {
  T* elements;                    // pointer to where the elements are in memory
  long size;                                // the number of elements in the set
  long capacity;                      // the current capacity (allocated memory)
} OrderedSet;

// -----------------------------------------------------------------------------

/*
  Function that creates an OrderedSet with elements of type T with a given
  initial capacity.
*/

OrderedSet* createOrderedSet(long initialCapacity) {

  OrderedSet* newSet = (OrderedSet*) malloc(sizeof(OrderedSet));

  newSet->size = 0;                        // initially, the set is empty
  newSet->capacity = initialCapacity;
  newSet->elements = (T*) malloc(initialCapacity * sizeof(T));

  return newSet;
}

// -----------------------------------------------------------------------------

/*
  Function that checks if an element exists in a given set. As the
  elements of the set are ordered in memory, this function uses binary
  search.

  Function returns 1 if the given element exists in the set, and 0
  otherwise.
*/

int contains(OrderedSet* set, const T element) {
  // TODO : Cerința 2
  long left = 0, high =  set->size - 1, middle;
  //T* elements = set->elements;
  while (left <= high) {
    middle = left + (high - left) / 2;
    if (set->elements[middle] == element) {
      return 1;
    } else if (set->elements[middle] > element) {
      high = middle - 1;
    } else {
      left = middle + 1;
    }
  }
  return 0;
}

// -----------------------------------------------------------------------------

/*
  Function that adds a new element to a set. If the element was
  already in the set, nothing changes. Otherwise, the element is added
  to the set. In the end, the elements must be in ascending order.
*/

void add(OrderedSet* set, const T newElement) {
  // TODO : Cerința 2
  long i;
  if (contains(set, newElement)) {
    return ;
  } else {
    if (set->size == set->capacity) {
      set->capacity *= 2;
      set->elements = (T*)realloc(set->elements, set->capacity);
    }
    set->size++;
    i = set->size - 1;
    while ((i >= 0) && (set->elements[i - 1] > newElement)) {
      set->elements[i] = set->elements[i - 1];
      i--;
    }
    set->elements[i] = newElement;
  }
  return ;
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the union of
  the two
*/

OrderedSet* unionOrderedSets(OrderedSet* s1, OrderedSet* s2) {
  // TODO : Cerința 3
  OrderedSet *unionSet = createOrderedSet(s1->capacity + s2->capacity);
  long idx1 = 0, idx2 = 0, idx = 0;
  while ((idx1 < s1->size) && (idx2 < s2->size)) {
    if (s1->elements[idx1] == s2->elements[idx2]) {
      unionSet->elements[idx++] = s1->elements[idx1++];
      idx2++;
    } else if (s1->elements[idx1] > s2->elements[idx2]) {
      unionSet->elements[idx++] = s2->elements[idx2++];
    } else {
      unionSet->elements[idx++] = s1->elements[idx1++];
    }
  }
  while (idx1 < s1->size) {
    unionSet->elements[idx++] = s1->elements[idx1++];
  }
  while (idx2 < s2->size) {
    unionSet->elements[idx++] = s2->elements[idx2++];
  }
  unionSet->size = idx;
  return unionSet;
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the
  intersection of the two
*/

OrderedSet* intersectOrderedSets(OrderedSet* s1, OrderedSet* s2) {
  // TODO : Cerința 3
    OrderedSet *interSet = createOrderedSet(s1->capacity);
  long idx1 = 0, idx2 = 0, idx = 0;
  while ((idx1 < s1->size) && (idx2 < s2->size)) {
    if (s1->elements[idx1] == s2->elements[idx2]) {
      interSet->elements[idx++] = s1->elements[idx1++];
      idx2++;
    } else if (s1->elements[idx1] > s2->elements[idx2]) {
      idx2++;
    } else {
      idx1++;
    }
  }
  interSet->size = idx;
  return interSet;
}


// -----------------------------------------------------------------------------

#endif
