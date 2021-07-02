#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Util.h"
#include "Graph.h"

TGraphL* createGraphAdjList(int numberOfNodes) {
	TGraphL* graph = (TGraphL*)malloc(sizeof(TGraphL));
	if (!graph) {
		printf("Nu s-a putut aloca memorie pentru graf\n");
		exit(1);
	}
	graph->nn = numberOfNodes;
	graph->adl = (ATNode*)malloc(numberOfNodes*sizeof(ATNode));
	if (!graph->adl){
		printf("Nu s-a putut aloca memorie pentru vectorul");
		printf(" ce retine nodurile grafului\n");
		free(graph);
		exit(1);
	}

	for(int i = 0; i < numberOfNodes; i++)graph->adl[i] = NULL;
	return graph;
}

void addEdgeList(TGraphL* graph, int v1, int v2) {
	TNode* n1 = (TNode*)malloc(sizeof(TNode));
	if (!n1) {
		printf("Nu s-a putut aloca memorie pentru nod\n");
		exit(1);
	}
	TNode* n2 = (TNode*)malloc(sizeof(TNode));
	if (!n2) {
		printf("Nu s-a putut aloca memorie pentru nod\n");
		free(n1);
		exit(1);
	}
	n1->v = v2;
	n1->c = 1;
	n1->next = graph->adl[v1];
	graph->adl[v1] = n1;

	n2->v = v1;
	n2->c = 1;
	n2->next = graph->adl[v2];
	graph->adl[v2] = n2;
}

List* dfsIterative(TGraphL* graph, int s) {

	List* path = createList();

	int* visited = (int*)malloc((graph->nn)*sizeof(int));
	if (!visited) {
		printf("Nu s-a putut aloca memorie pentru ");
		printf(" vectorul de vizitari\n");
		exit(1);
	}
	for (int i = 0; i < graph->nn; i++) {
		visited[i] = 0;
	}
	Stack* stack = createStack();
	ATNode aux;

	// introducem in stiva nodul
	// de start
	push(stack, s);
	// il marcam ca e si vizitat
	visited[s] = 1;

	while(!isStackEmpty(stack)){
		s = top(stack); // extragem varful stivei
		pop(stack); // il si eliminam

		enqueue(path, s);

		for(aux = graph->adl[s]; aux!=NULL; aux=aux->next){
			if(!visited[aux->v]) {
				visited[aux->v] = 1;
				push(stack, aux->v);
			}
		}
	}
	destroyStack(stack);
	free(visited);

	return path;
}

void dfsRecHelper(TGraphL* graph, int* visited, List* path, int s) {
	visited[s] = 1;

	enqueue(path, s);
	ATNode aux;

	for(aux = graph->adl[s]; aux != NULL; aux=aux->next){
		if(!visited[aux->v]) {
			dfsRecHelper(graph, visited, path, aux->v);
		}
	}

}

List* dfsRecursive(TGraphL* graph, int s) {
	List* path = createList();

	int* vizited = (int*)malloc(graph->nn*sizeof(int));
	for (int i = 0; i < graph->nn; i++) vizited[i] = 0;
	dfsRecHelper(graph, vizited, path, s);

	free(vizited);
	return path;
}

List* bfs(TGraphL* graph, int s){
	List* path = createList();

	int* visited = (int*)malloc((graph->nn)*sizeof(int));
	if (!visited) {
		printf("Nu s-a putut aloca memorie pentru ");
		printf(" vectorul de vizitari\n");
		exit(1);
	}
	for (int i = 0; i < graph->nn; i++) {
		visited[i] = 0;
	}
	Stack* q = createQueue();
	ATNode aux;

	// introducem in stiva nodul
	// de start
	enqueue(q, s);
	// il marcam ca e si vizitat
	visited[s] = 1;

	while(!isQueueEmpty(q)){
		s = front(q); // extragem varful stivei
		dequeue(q); // il si eliminam

		enqueue(path, s);

		for(aux = graph->adl[s]; aux!=NULL; aux=aux->next){
			if(!visited[aux->v]) {
				visited[aux->v] = 1;
				enqueue(q, aux->v);
			}
		}
	}
	destroyQueue(q);
	free(visited);

	return path;
}


void destroyGraphAdjList(TGraphL* graph){
	int i;
	TNode* aux, *aux2;
	for (i = 0; i < graph->nn; i++){
		aux = graph->adl[i];
		while(aux!=NULL) {
			aux2 = aux;
			aux = aux->next;
			free(aux2);
		}
	}
	free(graph->adl);
	free(graph);
}

void removeEdgeList(TGraphL* graph, int v1, int v2){
	TNode* it = graph->adl[v1];
	TNode* prev = NULL;
	while(it != NULL && it->v != v2){
		prev = it;
		it = it->next;
	}

	if(it == NULL)return;

	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v1] = it->next;
	free(it);

	it = graph->adl[v2];
	prev = NULL;
	while(it != NULL && it->v != v1){
		prev = it;
		it = it->next;
	}
	if(it == NULL) return;
	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v2] = it->next;

	free(it);
}

void removeNodeList(TGraphL* graph, int v){
	for(int i = 0; i < graph->nn;i++){
		removeEdgeList(graph,v,i);
	}
}
