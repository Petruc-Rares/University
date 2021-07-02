// Copyright @PetrucRares, Created on: May 8, 2020 necessaryADT.h

#ifndef necADT_H_
#define necADT_H_

typedef struct node{
	char* actorName;
	int actorNo;
	int v; // first element in list
	struct node* next; // field for colleagues which played on same movie
}Node;

typedef struct{
	int actorsNo;
	int filmsNo;
	Node **adl; 
}Graph;

typedef struct Queue{
	Node* front;
	Node* rear;
}Queue;

#endif /* necADT_H_ */