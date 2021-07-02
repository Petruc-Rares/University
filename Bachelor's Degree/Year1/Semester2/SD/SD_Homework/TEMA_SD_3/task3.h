// Copyright @PetrucRares, Created on: May 8, 2020 task3.h

#ifndef TASK3_H_
#define TASK3_H_

#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct helpStruct {
	int bridgesNo;
	char** names;
} helpStruct;

void dfsB(Graph* graph, Node* v, int timp, int* idx, int* low, Node** PI, helpStruct* auxStruct) {
	Node* it = v->next;

	idx[v->actorNo-1] = timp;
	low[v->actorNo-1] = timp;
	timp = timp + 1;

	while (it) {
		if ((PI[v->actorNo-1] == NULL) || (it->actorNo != PI[v->actorNo-1]->actorNo)) {
			if (idx[it->actorNo-1] == -1) {
				PI[it->actorNo-1] = v;
				dfsB(graph, graph->adl[it->actorNo-1], timp, idx, low, PI, auxStruct);
				low[v->actorNo-1] = MIN(low[v->actorNo-1], low[it->actorNo-1]);
				if (low[it->actorNo-1] > idx[v->actorNo-1]) {
					auxStruct->names = (char**) realloc(auxStruct->names, (auxStruct->bridgesNo+1)*sizeof(char*));
					auxStruct->names[auxStruct->bridgesNo] = (char*) malloc(MAXACTORNAME);
					if (strcmp(it->actorName, v->actorName) > 0) {
						strncpy(auxStruct->names[auxStruct->bridgesNo], v->actorName, MAXACTORNAME);
						strncat(auxStruct->names[auxStruct->bridgesNo], " ", 1);
						strncat(auxStruct->names[auxStruct->bridgesNo], it->actorName, MAXACTORNAME);
					} else {
						strncpy(auxStruct->names[auxStruct->bridgesNo], it->actorName, MAXACTORNAME);
						strncat(auxStruct->names[auxStruct->bridgesNo], " ", 1);
						strncat(auxStruct->names[auxStruct->bridgesNo], v->actorName, MAXACTORNAME);
					}
					auxStruct->bridgesNo++;
				}
			} else {
				low[v->actorNo-1] = MIN(low[v->actorNo-1], idx[it->actorNo-1]);
			}
		}
		it = it->next;
	}
	return ;
}

helpStruct* bridges(Graph* graph) {
	int timp = 0;
	int* idx, *low;
	Node** PI;
	helpStruct* auxStruct;

	idx = (int*) malloc(graph->actorsNo * sizeof(int));
	low = (int*) malloc(graph->actorsNo * sizeof(int));
	PI = (Node**) malloc(graph->actorsNo * sizeof(Node*));
	auxStruct = (helpStruct*) malloc(sizeof(helpStruct));
	auxStruct->bridgesNo = 0;
	auxStruct->names = NULL;

	for (int i = 0; i < graph->actorsNo; i++) {
		idx[i] = -1;
		low[i] = 200000000;
		PI[i] = NULL;
	}

	for (int i = 0; i < graph->actorsNo; i++) {
		if (idx[i] == -1) 
			dfsB(graph, graph->adl[i], timp, idx, low, PI, auxStruct);
	}

	qsort(auxStruct->names, auxStruct->bridgesNo, sizeof(auxStruct->names[0]), cmp);

	free(PI);
	free(idx);
	free(low);
	return auxStruct;
}

void deallocHelpStruct(helpStruct* auxStruct) {
	for (int i = 0; i < auxStruct->bridgesNo; i++) {
		free(auxStruct->names[i]);
	}
	free(auxStruct->names);
	free(auxStruct);
}

void printOutputTask3(helpStruct* auxStruct, FILE* out) {
	fprintf(out, "%d\n", auxStruct->bridgesNo);

	for (int i =0; i < auxStruct->bridgesNo; i++) {
		fprintf(out, "%s\n", auxStruct->names[i]);
	}
}

void task3(char* inputFile, char *outputFile) {
	FILE* in = openFileForRead(inputFile);
	// get the number of films (first element in the file)
	int filmsNo = getFilmsNo(in);
	Graph* graph = createGraph(MAXACTORS, filmsNo);

	// create the graph according to the movies the actors
	// played together
	fillGraph(&graph, in);

	helpStruct* auxStruct = bridges(graph);

	FILE* out = openFileForWriting(outputFile);
	printOutputTask3(auxStruct, out); // print in the desired format

	deallocHelpStruct(auxStruct);
	destroyGraph(graph);
	fclose(in);
	fclose(out);
}

#endif /* TASK3_H_ */