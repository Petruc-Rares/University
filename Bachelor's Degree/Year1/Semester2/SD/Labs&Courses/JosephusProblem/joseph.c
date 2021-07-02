#include <stdio.h>
#include <stdlib.h>

typedef struct Person {
  int id;
  struct Person* next;
} Person;

void printLista(Person *Circle) {
  Person *p = Circle;
  printf("%d ", p->id);
  p = p->next;
  while (p != Circle) {
    printf("%d ", p->id);
    p = p->next;
  }
  printf("\n");
}

Person* jelim(Person* p, int m) {
  Person *prev = NULL;
  for(int i = 1; i < m; i++) {
    prev = p;
    p = p->next;  
  }
  printf("Am eliminat pe %d\n", p->id);
  Person *aux = p->next;
  prev->next = p->next;
  free(p);
  return aux;
}

void chooseLeader(Person* Circle, int m) {
  Person *p = Circle;
  printf("Lista initiala\n");
  printLista(p);
  printf("\n");
  printf("Eliminare\n");
  while(p!=p->next) {
    p = jelim(p, m);
    printLista(p);
  }
  free(p);
  return ;
}

void insertForward(Person **Circle, int value) { 
  Person* newPerson = (Person*) malloc(sizeof(Person));
  if (!newPerson) {
    printf("Nu s-a putut realiza alocarea");
    exit(1);
  } else {
    newPerson->id = value;
    newPerson->next = *Circle;
    *Circle = newPerson;
  }
  return ;
}

Person* generateCircularList(int a, int b) {
  int i;
  Person* CL = NULL;
  for (i = b; i >= a; i--) {
    insertForward(&CL, i); 
  }
  Person *p;
  p = CL;
  while (p->next) {
    p = p->next;
  }
  p->next = CL;
  return CL;
}

int main() {
  chooseLeader(generateCircularList(1, 9), 5);
}
