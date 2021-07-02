// Copyright @PetrucRares, Created on: May 8, 2020 task1.h

#ifndef TASK1_H_
#define TASK1_H_

#define MAXACTORS 5000
#define MAXFILMNAME 500
#define MAXACTORNAME 200

void enqueue(Queue *q, int x, char* name){
	Node *newNode = (Node*) malloc(sizeof(Node));
	newNode->actorNo = x;
	newNode->actorName = (char*) malloc(MAXACTORNAME);
	newNode->next = NULL;
	strncpy(newNode->actorName, name, MAXACTORNAME);
	if(isEmptyQueue(q)){
		q->front = q->rear = newNode;
	} else{
		q->rear->next = newNode;
		q->rear = newNode;
	}
	return;
}

char* frontActorName(Queue* q) {
	return q->front->actorName;
}

Node* visitBfs(int s, char* name_s, int* visited, Graph* graph) {
	char *name;
	Queue*q, *path;
	initQueue(&q);
	initQueue(&path);
	Node* aux, *aux2;
	int contor = 0;

	// insert the start node in the stack
	enqueue(q, s, name_s);

	// mark it as visited
	visited[s-1] = 1;

	while(!isEmptyQueue(q)){
		contor++;
		s = frontActorNo(q); // extract actor number from the top node
		name = frontActorName(q); // extract actor name from the top node

		// add a new node in the new conex subgraph containing the previous
		// extracted values
		enqueue(path, s+1, name);

		aux2 = graph->adl[s]->next;

		dequeue(q); // remove the element from the queue

		for(aux = aux2; aux!=NULL; aux=aux->next){
			if(!visited[aux->actorNo-1]) {
				visited[aux->actorNo-1] = 1;
				enqueue(q, aux->actorNo, aux->actorName);
			}
		}
	}
	aux = path->front;
	
	// hold the number of elements in conex subgraph aux->v
	aux->v = contor;	
	free(path);
	destroyQueue(q);
	return aux;
}

Node* bfs(Graph* graph){
	Node *path = NULL, *aux, *maybePath;
	int max = -1;

	int* visited = (int*)malloc((graph->actorsNo)*sizeof(int));
	if (!visited) {
		printf("Allocation failed for visit array \n");
		exit(1);
	}

	// 0 means not visited
	for (int i = 0; i < graph->actorsNo; i++) {
		visited[i] = 0;
	}


	// maybePath = might be the list we are looking for
	// path = list we are looking for
	for (int i = 0; i < graph->actorsNo; i++) {
		if (visited[i] == 0) {
			maybePath = visitBfs(graph->adl[i]->actorNo, graph->adl[i]->actorName, visited, graph);
			// if the new path generated has less elements than the previous one
			// we ignore it, so we dealloc the memory for it.
			if (maybePath->v < max) {
				while (maybePath) {
					aux = maybePath;
					maybePath = maybePath->next;
					free(aux->actorName);
					free(aux);	
				}
			}
			// else we have to dealloc the memory occupied by the previous
			// biggest list, and assign the new "biggest" values.
			 else {
			 	while (path) {
					aux = path;
					path = path->next;
					free(aux->actorName);
					free(aux);	
				}
				max = maybePath->v;
				path = maybePath;
			}
		}
	}
	free(visited);
	return path;
}

int getFilmsNo(FILE* in) {
	int no;
	fscanf(in, "%d", &no);
	return no;
}

int cmp(const void*a, const void*b) {
	return strcmp(*(const char**)a, *(const char**)b);
}

void printToFile(FILE* out, int auxNo, char** names) {
	fprintf(out, "%d\n", auxNo);
	for (int i = 0; i < auxNo; i++) {
		fprintf(out, "%s\n", names[i]);
	}
	return;
}

void destroyStrings(char **names, int auxNo) {
	for (int i = 0; i < auxNo; i++) {
		free(names[i]);
	}
	return;
}

void task1(char* inputFile, char *outputFile) {
	// open input File
	FILE* in = openFileForRead(inputFile);
	// get the number of films (first element in the file)
	int filmsNo = getFilmsNo(in), auxNo, contor = 0;
	Graph* graph = createGraph(MAXACTORS, filmsNo);

	// create the graph according to the movies the actors
	// played together
	fillGraph(&graph, in);
	//printInfo(graph);

	// q is the biggest convex subgraph
	Node* q = bfs(graph);
	// auxNo holds the number of elements in the subgraph
	// as q will be destroyed and q->v can not be accessed anymore
	auxNo = q->v;
	Node* aux;
	char *names[q->v];

	// destroy the list containing the biggest conex subgraph
	// and create an array of strings with names in the list
	while(q) {
		aux = q;
		q = q->next;
		names[contor] = (char*) malloc(MAXACTORNAME);
		strncpy(names[contor], aux->actorName, MAXACTORNAME);
		free(aux->actorName);
		free(aux);
		contor++;
	}

	FILE* out = openFileForWriting(outputFile);
	// arrange the names lexicographical
	qsort(names, auxNo, sizeof(char*), cmp);
	// print to file the things that are required
	printToFile(out, auxNo, names);

	destroyStrings(names, auxNo);
	destroyGraph(graph);
	fclose(in);
	fclose(out);
	return;
}

#endif /* TASK1_H_ */