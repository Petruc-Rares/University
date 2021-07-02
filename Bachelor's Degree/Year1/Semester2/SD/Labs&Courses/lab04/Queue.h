#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode{
	Item elem;
	struct QueueNode *next;
}QueueNode;

typedef QueueNode TcelQ;
typedef QueueNode *ACelQ;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

typedef Queue TCoada;
typedef Queue *AQ;


Queue* createQueue(void){
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	if (!queue) {
		printf("Nu s-a putut aloca memorie pentru coada");
		exit(1);
	} else {
		queue->front = queue->rear = NULL;
		queue->size = 0;
	}
	return queue;
} 

int isQueueEmpty(Queue *q){
	return (q == NULL || q->front == NULL);
	return 0;
}

void enqueue(Queue *q, Item elem){
	if (!q) return;
	ACelQ lastNode = (ACelQ) malloc(sizeof(TcelQ));
	if (!lastNode) {
		printf("Nu s-a putut aloca memorie pentru nodul introdus la coada\n");
		exit(1);
	} else {
		lastNode->elem = elem;
		lastNode->next = NULL;
		if (q->front == NULL) {
			q->rear = q->front = lastNode;
			q->size = 1;
			return ;
		}
		q->rear->next = lastNode;
		q->rear = lastNode;
	}
	q->size++;
	return ;
}

Item front(Queue* q){
	if (!q) return 0; // default value
	if (q->front == NULL) return 0;
	return q->front->elem;
}

void dequeue(Queue* q){
	if (!q) return;
	if (q->front == NULL) return;
	// in caz de ramane un singur nod in lista
	if (q->front->next == NULL) {
		q->rear = NULL;
	}
	ACelQ aux = q->front;
	q->front = q->front->next;
	if (q->front != NULL) {

		if (q->front->next == NULL) {

			q->rear = q->front;

		}

	}
	q->size--;
	free(aux);
	return;
}

void destroyQueue(Queue *q){
	if (q->front == NULL) return ;
	ACelQ aux;
	while (q->front) {
		aux = q->front;
		q->front = q->front->next;
		free(aux);
	}
	free(q);
	return ;
	// TODO: Cerinta 2
}

#endif
