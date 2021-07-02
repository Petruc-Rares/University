// Copyright @PetrucRares, Created on: May 8, 2020 task2.h

#ifndef TASK2_H_
#define TASK2_H_

// returns index of the actor, when name is found
// returns -1 if actor name is not found 
int getIndexAfterName(Graph* graph, char name[MAXACTORNAME]) {
	int i;
	for (i = 0; i < graph->actorsNo; i++) {
		if (strcmp(graph->adl[i]->actorName, name) == 0) return i;
	}
	if (i == graph->actorsNo) return -1;
	// case: graph->actorsNo = 0
	return -1;
}

void enqueueNormal(Queue *q, int x){
	Node *newNode = (Node*) malloc(sizeof(Node));
	newNode->actorNo = x;
	newNode->actorName = NULL;
	newNode->next = NULL;
	if(isEmptyQueue(q)){
		q->front = q->rear = newNode;
	} else{
		q->rear->next = newNode;
		q->rear = newNode;
	}
	return;
}


int min_bfs(Graph* graph, int index1, int index2) {

	int* visited = (int*) malloc(sizeof(int) * graph->actorsNo), s;
	if (!visited) {
		printf("Allocation failed for visited array\n");
		exit(1);
	}

	int *distances = (int*) malloc(sizeof(int) *graph->actorsNo);
	if (!distances) {
		printf("Allocation failed for distances array\n");
		exit(1);
	}
	int auxNo;

	Queue*q;
	Node* aux, *aux2;
	initQueue(&q);

	enqueueNormal(q, index1+1);

	for (int i = 0; i < graph->actorsNo; i++) {
		visited[i] = 0;
		distances[i] = 0;
	}

	visited[index1] = 1;

	while(!isEmptyQueue(q)){
		s = frontActorNo(q); // extract actor number from the top node

		aux2 = graph->adl[s]->next;

		dequeue(q); // remove the element from the queue

		for(aux = aux2; aux!=NULL; aux=aux->next){
			if(!visited[aux->actorNo-1]) {
				visited[aux->actorNo-1] = 1;
				distances[aux->actorNo-1] = distances[s] + 1;
				// if we reached the actor we are interested in
				// return the distance (do not make more calculations)
				if (aux->actorNo - 1 == index2) {
					auxNo = distances[aux->actorNo-1];
					free(visited);
					free(distances);
					destroyQueue(q);
					return auxNo;
				}
				enqueueNormal(q, aux->actorNo);
			}
		}
	}

	auxNo = distances[index2];
	free(visited);
	free(distances);
	destroyQueue(q);
	// test if the nodes can be linked through the graph
	// if they can not be linked, return -1
	if (auxNo != 0) return auxNo;
	return -1;
}

void task2(char* inputFile, char *outputFile) {
	FILE* in = openFileForRead(inputFile);
	// get the number of films (first element in the file)
	int filmsNo = getFilmsNo(in), index1, index2, answer;
	Graph* graph = createGraph(MAXACTORS, filmsNo);

	// create the graph according to the movies the actors
	// played together
	fillGraph(&graph, in);

	// read the last two names of the actors we are
	// interested in
	char actor1[MAXACTORNAME], actor2[MAXACTORNAME];
	fseek(in, 1, SEEK_CUR);
	fscanf(in, "%[^\n]", actor1);
	fseek(in, 1, SEEK_CUR);
	fscanf(in, "%[^\n]", actor2);

	// get the indexes in the adjacency list for the
	// names of the actors
	index1 = getIndexAfterName(graph, actor1);
	if (index1 == -1) {
		printf("The name of the actor was not found\n");
		destroyGraph(graph);
		fclose(in);
		return;
	}

	index2 = getIndexAfterName(graph, actor2);
	if (index2 == -1) {
		printf("The name of the actor was not found\n");
		destroyGraph(graph);
		fclose(in);
		return;
	}

	answer = min_bfs(graph, index1, index2);

	// print the number of edges between the actors
	FILE* out = openFileForWriting(outputFile);
	fprintf(out, "%d\n", answer);

	destroyGraph(graph);
	fclose(in);
	fclose(out);
	return;
}

#endif /* TASK2_H_ */