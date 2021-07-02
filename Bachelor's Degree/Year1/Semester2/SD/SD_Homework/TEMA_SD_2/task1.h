// Copyright @PetrucRares, Created on: May 8, 2020 task1.h

#ifndef TASK1_H_
#define TASK1_H_

#define MAXACTORS 200
#define MAXFILMNAME 100
#define MAXACTORNAME 38

typedef struct node{
	char* actorName;
	int actorNo;
	int v; // primul elem din lista
	struct node* next; // field for colleagues which played on same movie
}Node;

typedef struct{
	int actorsNo;
	int filmsNo;
	Node **adl; 
}Graph;

/* partea asta o vreau in alt fisier .c */

typedef struct Queue{
	Node* front;
	Node* rear;
}Queue;

void initQueue(Queue **q){
	(*q) = (Queue *) malloc(sizeof(Queue));
	(*q)->front = NULL;
	(*q)->rear = NULL;
}

int isEmptyQueue(Queue *q){
	return (q->front == NULL);
}

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

int frontActorNo(Queue *q){
	return q->front->actorNo-1;
}

char* frontActorName(Queue* q) {
	return q->front->actorName;
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

void destroyQueue(Queue *q){
	if (q==NULL)
		return ;
	while(!isEmptyQueue(q)){
		dequeue(q);
	}
	free(q);
	return;
}

void printQueue(Node* q) {
//	Node* q = qu->front;
	while(q != NULL) {
		printf("%d %s\n", q->actorNo, q->actorName);
		q = q->next;
	}
	return;
}

Node* visitBfs(int s, char* name_s, int* visited, Graph* graph) {
	char *name;
	Queue*q, *path;
	initQueue(&q);
	initQueue(&path);
	Node* aux, *aux2;
	int contor = 0;

	// introducem in stiva nodul
	// de start
	enqueue(q, s, name_s);
	// il marcam ca e si vizitat
	visited[s-1] = 1;

	while(!isEmptyQueue(q)){
		contor++;
		s = frontActorNo(q); // extragem varful stivei
		name = frontActorName(q);

		//printf("%d %s\n", s, name);

		enqueue(path, s+1, name);

		aux2 = graph->adl[s]->next;

		dequeue(q); // il si eliminam

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
	Node *path, *aux, *maybePath;
	int max = -1;

	int* visited = (int*)malloc((graph->actorsNo)*sizeof(int));
	if (!visited) {
		printf("Nu s-a putut aloca memorie pentru ");
		printf(" vectorul de vizitari\n");
		exit(1);
	}
	for (int i = 0; i < graph->actorsNo; i++) {
		visited[i] = 0;
	}

	for (int i = 0; i < graph->actorsNo; i++) {
		if (visited[i] == 0) {
			if (graph->adl[i]->actorName) {
			maybePath = visitBfs(graph->adl[i]->actorNo, graph->adl[i]->actorName, visited, graph);
			if (maybePath->v < max) {
				while (maybePath) {
					aux = maybePath;
					maybePath = maybePath->next;
					free(aux->actorName);
					free(aux);	
				}
			} else {
				max = maybePath->v;
				path = maybePath;
			}
		}
	}
}
	free(visited);
	return path;
}

/* pana aici trebuie mutat in alt fisier .c */



FILE* openFileForRead(char* inputFile) {
	FILE *fptr = fopen(inputFile, "r");
	if (!fptr) {
		printf("Input file can not be opened\n");
		exit(1);
	}
	return fptr;
} 

int getNumberOfNodes(FILE* in) {
	return 2;
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

void destroy(Graph* graph) {
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

int getFilmsNo(FILE* in) {
	int no;
	fscanf(in, "%d", &no);
	return no;
}

int contains(Node* iterator,  int value) {
	while ((iterator) && (iterator->actorNo != value)) {
		iterator = iterator->next;
	}
	if (iterator == NULL) return 0;
	else return 1;
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
		for (j = i + 1; j <actorNo; j++) {
			addEdgeList(*graph, auxArray[i] - 1, auxArray[j] - 1, names[i], names[j]);
		}
	}

	return;

}

void fillGraph(Graph** graph, FILE* in) {
	Graph* auxGraph = *graph;
	char filmName[MAXFILMNAME], actorName[MAXACTORNAME];
	char* auxActorNo = (char*) malloc(4 * sizeof(char));
	int actorsNo, i, j, totalActorsNo, max = -1, k = 0, p, q;
	for (i = 0; i < auxGraph->filmsNo; i++) {
		fscanf(in, "%s", filmName);
		fscanf(in, "%d", &actorsNo);
		// array containing the numbers at the end of each actor's name 
		int* auxArray = (int*) malloc(actorsNo*sizeof(int));
		// matrix containing on each line the name of each actor
		char **auxNames = (char**) malloc(actorsNo*sizeof(char*));
		for (q = 0; q < actorsNo; q++) auxNames[q] = (char*) malloc(MAXACTORNAME* sizeof(char));
		q = 0;
		//printf("%s %d\n", filmName, actorsNo);
		for (j = 0; j < actorsNo; j++) {
			fscanf(in, "%s", actorName);
			k = 0;
			for (p = 5; p < strlen(actorName); p++)
			{
				auxActorNo[k] = actorName[p];
				k++;
			}
			auxActorNo[k] = '\0';
			totalActorsNo = atoi(auxActorNo);
			//printf("%d\n", totalActorsNo);
			if (auxGraph->adl[totalActorsNo-1]->actorName == NULL) {
				auxGraph->adl[totalActorsNo-1]->actorName = (char*) malloc(MAXACTORNAME);
				strncpy(auxGraph->adl[totalActorsNo-1]->actorName, actorName, MAXACTORNAME);
			}
			if (totalActorsNo > max) max = totalActorsNo;
			auxArray[q] = totalActorsNo;
			strcpy(auxNames[q], actorName);
			q++;
		}
		// create the graph according to the links
		makeLinks(graph, auxArray, actorsNo, auxNames);
		free(auxArray);
		for (q = 0; q <actorsNo; q++) free(auxNames[q]);
		free(auxNames);
	}
	free(auxActorNo);
	// destroy everything in graph after the last actor
	for (i = max; i < auxGraph->actorsNo; i++) {
		free(auxGraph->adl[i]);
	}
	auxGraph->actorsNo = max;
	return;
}

void printInfo(Graph* graph) {
	int i;
	Node* it;
	for (i= 0; i < graph->actorsNo; i++) {
		//printf("Actorul cu numele %s\n", graph->adl[i]->actorName);
		//printf("Actorul cu numarul %d\n", i+1);
		it = graph->adl[i]->next;
		while (it) {
			//printf("joaca in acelasi film cu actorul cu numarul %d\n", it->actorNo);
			//printf("joaca in acelasi film cu actorul cu numarul %s\n", it->actorName);
			it = it->next;
		}
	}
	return;
}

void task1(char* inputFile, char *outputFile) {
	FILE* in = openFileForRead(inputFile);
	int filmsNo = getFilmsNo(in);
	Graph* graph = createGraph(MAXACTORS, filmsNo);	
	printf("%d\n", filmsNo);
	fillGraph(&graph, in);
	printInfo(graph);
	printf("HAIREJRIAEJRILAERJLAE\n");
	printf("INCEPE PARTEA A 2-A\n\n\n");
	Node* q = bfs(graph);
	printf("HAIREJRIAEJRILAERJLAE\n");
	printf("%d\n\n", q->v);
	printQueue(q);
	//printf("WELCOME TO TASK NUMBER 1 WITH THE FILES:\n input: %s\n output: %s\n %d\n", inputFile, outputFile, x);
	/* se va face functie separata pentru asta */
	while(q) {
		Node* aux = q;
		q = q->next;
		free(aux->actorName);
		free(aux);
	}
	/* */
	destroy(graph);
	fclose(in);
	return;
}

#endif /* TASK1_H_ */