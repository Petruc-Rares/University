// Copyright @PetrucRares, Created on: May 8, 2020 commonFunctions.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "necessaryADT.h"

#define MAXACTORS 10000
#define MAXFILMNAME 200
#define MAXACTORNAME 100

FILE* openFileForRead(char* inputFile) {
	FILE *fptr = fopen(inputFile, "r");
	if (!fptr) {
		printf("Input file can not be opened\n");
		exit(1);
	}
	return fptr;
} 

FILE* openFileForWriting(char* outputFile) {
	FILE *fptr = fopen(outputFile, "w");
	if (!fptr) {
		printf("Input file can not be opened\n");
		exit(1);
	}
	return fptr;
}

void initQueue(Queue **q){
	(*q) = (Queue *) malloc(sizeof(Queue));
	(*q)->front = NULL;
	(*q)->rear = NULL;
}

int isEmptyQueue(Queue *q){
	return (q->front == NULL);
}

int frontActorNo(Queue *q){
	return q->front->actorNo-1;
}

void dequeue(Queue *q){
	if(isEmptyQueue(q))
		return;

	Node *del = q->front;
	if( q->front == q->rear )
		q->front = q->rear = NULL;
	else
		q->front = q->front->next;
	free(del->actorName);
	free(del);
	return;
}

void printQueue(Node* q){
//	Node* q = qu->front;
	while(q != NULL) {
		printf("%d %s\n", q->actorNo, q->actorName);
		q = q->next;
	}
	return;
}

void destroyQueue(Queue *q){
	if (q==NULL)
		return ;
	while(!isEmptyQueue(q)){
		dequeue(q);
	}
	free(q);
	return;
}

int contains(Node* iterator,  int value) {
	while ((iterator) && (iterator->actorNo != value)) {
		iterator = iterator->next;
	}
	if (iterator == NULL) return 0;
	else return 1;
}

Graph* createGraph(int actorsNo, int filmsNo) {
	Graph* graph = (Graph*) malloc(sizeof(Graph));
	if (!graph) {
		printf("Allocation failed for graph\n");
		exit(1);
	}
	graph->actorsNo = actorsNo;
	graph->filmsNo = filmsNo;
	graph->adl = (Node**) malloc(actorsNo*sizeof(Node*));
	if (!graph->adl) {
		free(graph);
		printf("Allocation failed for adjacency list\n");
		exit(1);
	}
	for (int i = 0; i < actorsNo; i++) {
		graph->adl[i] = (Node*) malloc(sizeof(Node));
		if (!graph->adl[i]) {
			printf("Allocation failed for node in adjacency list\n");
			for (int j = 0; j < i; j++) free(graph->adl[j]);
			free(graph->adl);
			free(graph);
			exit(1);
		}
		graph->adl[i]->actorNo = i+1;
		graph->adl[i]->actorName = NULL;
		graph->adl[i]->next = NULL;
	}
	return graph;
}

void destroyGraph(Graph* graph) {
	int i;
	Node* it, *aux;
	for (i = 0; i < graph->actorsNo; i++) {
		it = graph->adl[i];
		while(it != NULL) {
			aux = it;
			it = it->next;
			free(aux->actorName);
			free(aux);
		}
	}
	free(graph->adl);
	free(graph);
	return;
}

void addEdgeList(Graph* graph, int v1, int v2, char* name1, char* name2) {
	Node* n1 = (Node*)malloc(sizeof(Node));
	if (!n1) {
		printf("Nu s-a putut aloca memorie pentru nod\n");
		exit(1);
	}
	Node* n2 = (Node*)malloc(sizeof(Node));
	if (!n2) {
		printf("Nu s-a putut aloca memorie pentru nod\n");
		free(n1);
		exit(1);
	}

	if (!contains(graph->adl[v1]->next, v2+1)){
		n1->actorNo = v2+1;
		n1->actorName = (char*) malloc(MAXACTORNAME);
		strncpy(n1->actorName, name2, MAXACTORNAME);
		n1->v = v1+1;
		n1->next = graph->adl[v1]->next;
		graph->adl[v1]->next = n1;
	} else free(n1);

	if (!contains(graph->adl[v2]->next, v1+1)){
		n2->actorNo = v1+1;
		n2->actorName = (char*) malloc(MAXACTORNAME);
		strncpy(n2->actorName, name1, MAXACTORNAME);
		n2->v = v2+1;
		n2->next = graph->adl[v2]->next;
		graph->adl[v2]->next = n2;
	} else free(n2);
}

void makeLinks(Graph** graph, int* auxArray, int actorNo, char** names) {
	int i, j;
	for (i = 0; i < actorNo-1; i++) {
		for (j = i + 1; j < actorNo; j++) {
			addEdgeList(*graph, auxArray[i], auxArray[j], names[i], names[j]);
		}
	}

	return;

}

int actorNameIsUnique(Graph* graph, char* name, int totalActorsNo){
	if (graph->adl[0]->actorName){
		if (strcmp(graph->adl[0]->actorName, name) == 0) return -1;
	}
	for (int i = 1; i <totalActorsNo; i++) {
		if (strcmp(graph->adl[i]->actorName, name) == 0) return i;
	}
	return 0;
}

void fillGraph(Graph** graph, FILE* in){
	Graph* auxGraph = *graph;
	char filmName[MAXFILMNAME], actorName[MAXACTORNAME];
	char* auxActorNo = (char*) malloc(4 * sizeof(char));
	int actorsNo, i, j, totalActorsNo = 0, q, indexActor;
	for (i = 0; i < auxGraph->filmsNo; i++) {
		fseek(in, 1, SEEK_CUR);
		fscanf(in, "%[^\n]", filmName);
		fseek(in, 1, SEEK_CUR);
		fscanf(in, "%d", &actorsNo);
		// array containing the indexes of the actors in the adjacency matrix
		int *auxArray = (int*) malloc(actorsNo*sizeof(int));
		// matrix containing on each line the name of each actor
		char **auxNames = (char**) malloc(actorsNo*sizeof(char*));
		for (q = 0; q < actorsNo; q++) 
			auxNames[q] = (char*) malloc(MAXACTORNAME* sizeof(char));
		q = 0;
		//printf("%s %d\n", filmName, actorsNo);
		for (j = 0; j < actorsNo; j++) {
			fseek(in, 1, SEEK_CUR);
			fscanf(in, "%[^\n]", actorName);
			// if the actor name is unique (indexActor = 0), we need to create
			// the node of the adjacency matrix corresponding to the new actor
			indexActor = actorNameIsUnique(auxGraph, actorName, totalActorsNo);
			if (indexActor == 0) {
				auxGraph->adl[totalActorsNo]->actorName = (char*) malloc(MAXACTORNAME);
				strncpy(auxGraph->adl[totalActorsNo]->actorName, actorName, MAXACTORNAME);
				indexActor = totalActorsNo;
				totalActorsNo++;
			}
			// if the actorName corresponds to the name in the first node of the
			// adjacency matrix, it is a special case, so indexActor = 0
			if(indexActor == -1) indexActor = 0;
			auxArray[q] = indexActor;
			strncpy(auxNames[q], actorName, MAXACTORNAME);
			q++;
		}
		// create the graph according to the links
		makeLinks(graph, auxArray, actorsNo, auxNames);
		free(auxArray);
		for (q = 0; q <actorsNo; q++) free(auxNames[q]);
		free(auxNames);
	}
	free(auxActorNo);
	// destroy everything in graph after the last actor (not neccessary nodes)
	for (i = totalActorsNo; i < auxGraph->actorsNo; i++) {
		free(auxGraph->adl[i]);
	}
	auxGraph->actorsNo = totalActorsNo;
	return;
}

void printInfo(Graph* graph) {
	int i;
	Node* it;
	for (i= 0; i < graph->actorsNo; i++) {
		//printf("Actorul cu numele %s\n", graph->adl[i]->actorName);
		printf("Actorul cu numarul %d\n", graph->adl[i]->actorNo);
		it = graph->adl[i]->next;
		while (it) {
			printf("joaca in acelasi film cu actorul cu numarul %d\n", it->actorNo);
			//printf("joaca in acelasi film cu actorul cu numarul %s\n", it->actorName);
			it = it->next;
		}
	}
	return;
}