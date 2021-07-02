#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>



typedef struct StackNode{
	Item elem;
	struct StackNode *next;
}StackNode;

typedef StackNode TCelSt; // Redenumirea unui nod la notatiile din curs 
typedef StackNode *ACelSt; // Redenumirea unui pointer la nod la notatiile din curs

typedef struct Stack{
	StackNode* head;  // Varful stivei
	long size; // Numarul de elemente din stiva
}Stack;

typedef Stack TStiva; // Redenumirea stivei la notatiile din curs
typedef Stack *ASt;//  // Redenumirea unui pointer la stiva la notatiile din curs

Stack* createStack(void){
	Stack* stack = (Stack*) malloc(1 * sizeof(Stack));
	if (!stack) {
		printf("Nu s-a putut realiza alocarea stivei");
		exit(1);
	} else {
		stack->head = NULL;
		stack->size = 0;
	}
	return stack;
}

int isStackEmpty(Stack* stack){
	if (stack != NULL) {
		if (stack->head == NULL) return 1;
	    return 0;
	} 
	return 0;
}

void push(Stack *stack, Item elem){
	StackNode* newHead = (StackNode*) malloc(1 * sizeof(StackNode));
	if (!newHead) {
		printf("Nu s-a putut realiza alocarea noului cap al stivei");
		exit(1);
	}
	newHead->elem = elem;
	newHead->next = stack->head;
	stack->head = newHead;
	stack->size++;
}

Item top(Stack *stack){	
	if (stack != NULL && stack->head != NULL) return 0; // la ce ar trebui sa dau return aici?
	return stack->head->elem;
} 

void pop(Stack *stack){
	if (!stack) return;
	StackNode* aux = stack->head;
	if (stack->head) {
		stack->head = stack->head->next;
	} else return ;
	stack->size--;
	free(aux);
	return;
}

void destroyStack(Stack *stack){
	// TODO: Cerinta 1
	if (!stack) return;
	StackNode* aux;
	while (stack->head != NULL) {
		// pop(stack) cat timp st
		aux = stack->head;
		stack->head = stack->head->next;
		free(aux);
	}
	free(stack);
	return ;
}

#endif 
