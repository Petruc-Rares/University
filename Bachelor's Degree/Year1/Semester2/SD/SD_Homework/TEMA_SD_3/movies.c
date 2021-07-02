// Copyright @PetrucRares, Created on: May 8, 2020 movies.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "necessaryADT.h"

// declare the common functions for the tasks
FILE* openFileForRead(char* inputFile);
FILE* openFileForWriting(char* outputFile);
void initQueue(Queue **q);
int isEmptyQueue(Queue *q);
int frontActorNo(Queue *q);
void dequeue(Queue *q);
void printQueue(Node* q);
void destroyQueue(Queue *q);
Graph* createGraph(int actorsNo, int filmsNo);
void destroyGraph(Graph* graph);
void addEdgeList(Graph* graph, int v1, int v2, char* name1, char* name2);
void fillGraph(Graph** graph, FILE* in);
int actorNameIsUnique(Graph* graph, char* name, int totalActorsNo);
void makeLinks(Graph** graph, int* auxArray, int actorNo, char** names);
void printInfo(Graph* graph);
int contains(Node* iterator,  int value);

#include "task1.h"
#include "task2.h"
#include "task3.h"

int main(int argc, char*argv[]) {
	// we are looking for the task that is wanted to be evaluated
	if (strcmp(argv[1], "-c1") == 0) task1(argv[2], argv[3]);
	else if (strcmp(argv[1], "-c2") == 0) task2(argv[2], argv[3]);
	else if (strcmp(argv[1], "-c3") == 0) task3(argv[2], argv[3]);
	// SPECIAL CASE (if the task required to be solved is not one of the above)
	else {
		printf("Task %s is not valid\n", argv[1]);
		return 2;
	}
	return 0;
}