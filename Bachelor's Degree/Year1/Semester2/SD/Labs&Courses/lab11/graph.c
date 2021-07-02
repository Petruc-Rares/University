#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"

typedef int TCost;

typedef struct node
{
	int v;
	TCost c;
	struct node *next;
} TNode, *ATNode;

typedef struct
{
	int nn;
	ATNode *adl;
}	TGraphL;


void alloc_list(TGraphL * G, int n)
{
    int i;
    G->nn = n;
	G->adl = (ATNode*) malloc((n+1)*sizeof(ATNode));
	for(i = 0; i < n; i++)
		G->adl[i] = NULL;

}

void dealloc_list(TGraphL G) {
	int i;
	ATNode it, aux;
	for (i = 0; i < G.nn; i++) {
		for(it =  G.adl[i]; it != NULL; it = it->next) {
			aux = it;
			it = it->next;
			free(aux);
		}
	}
	//free(G.adl);
}

void free_list(TGraphL *G)
{
    int i;
    ATNode it;
    for(i = 0; i < G->nn; i++){
		it = G->adl[i];
		while(it != NULL){
			ATNode aux = it;
			it = it->next;
			free(aux);
		}
		G->adl[i] = NULL;
	}
	free(G->adl);
	G->nn = 0;
}

void insert_edge_list(TGraphL *G, int v1, int v2, int c)
{
 TNode *t;
 t=(ATNode)malloc(sizeof(TNode));
 t->v = v2; t->c=c; t->next = G->adl[v1]; G->adl[v1]=t;
 t=(ATNode)malloc(sizeof(TNode));
 t->v = v1;  t->c=c; t->next = G->adl[v2]; G->adl[v2]=t;
}

void dealloc_queue(MinHeap* heap) {
	free(heap->elem);
	free(heap->pos);
	free(heap);
	return;
}


void dijkstra(TGraphL G, int s)
{
    MinHeap* minheap = newQueue(G.nn);
    int i, d[G.nn], parents[G.nn];
    MinHeapNode *u= NULL;
    ATNode it;

    for (i = 0; i < G.nn; i++) {
    	d[i] = INT_MAX;
    	parents[i] = -1;
    	minheap->elem[i] = newNode(i, d[i]);
    	minheap->pos[i] = i;
    }

    free(minheap->elem[s]);
    d[s]=0;
    minheap->elem[s] = newNode(s, d[s]);

    SiftUp(minheap, s, d[s]);
    
    minheap->size = G.nn;
/*
				for (i = 0; i < G.nn; i++)
					printf("%d ", minheap->pos[i]);

				printf("\n");

				for (i = 0; i < G.nn; i++)
					printf("%d ", minheap->elem[i]->d);			

				printf("\n\n");
*/
    while (isEmpty(minheap) == 0) {
		u = removeMin(minheap);

/*

				for (i = 0; i < G.nn; i++)
					printf("%d ", minheap->pos[i]);

				printf("\n");

				for (i = 0; i < G.nn; i++)
					printf("%d ", minheap->elem[i]->d);			

				printf("\n\n");
*/
		for (it = G.adl[u->v]; it != NULL; it = it->next) {

			if ((isInMinHeap(minheap, it->v)) &&
				(d[it->v] > d[u->v]+it->c)) {

				parents[it->v] = u->v;
				d[it->v] = d[u->v]+it->c;
    			SiftUp(minheap, it->v, d[it->v]);

				/*printf("Nod curent: %d |", it->v);

				for (i = 0; i < G.nn; i++)
					printf("%d ", minheap->elem[i]->v);

				printf("\n");

				for (i = 0; i < G.nn; i++)
					printf("%d ", minheap->pos[i]);			

				printf("\n\n");*/
    		}

		}

		free(u);
    }

    printf("Varf\tDrum minim\n");
    for (i = 0; i < G.nn; i++) {
    	printf("%d\t%d\t %d\n", i, d[i], parents[i]);
    }

    dealloc_queue(minheap);

    return;
}

void Prim(TGraphL G)
{
	int parents[G.nn], d[G.nn], i;  

   	MinHeap* minheap = newQueue(G.nn);
    MinHeapNode *u= NULL;
    ATNode it;

    for (i = 0; i < G.nn; i++) {
    	d[i] = INT_MAX;
    	parents[i] = -1;
    	minheap->elem[i] = newNode(i, d[i]);
    	minheap->pos[i] = i;
    }

    minheap->size = G.nn;

//		for (int i = 0; i < minheap->capacity; i++)
	//		printf("%d ", minheap->pos[i]);

	//	printf("\n\n");

    while (isEmpty(minheap) == 0) {
		u = removeMin(minheap);

	//	printf("varful: %d\n", u->v);

	//	for (int i = 0; i < minheap->capacity; i++)
	//		printf("%d ", minheap->pos[i]);

	//	printf("\nInainte de SIFTUP\n");


		for (it = G.adl[u->v]; it != NULL; it = it->next) {

			if ((isInMinHeap(minheap, it->v)) &&
				(d[it->v] > it->c)) {
				parents[it->v] = u->v;
				d[it->v] = it->c;
 			//	printf("%d %d\n", it->v, d[it->v]);
    			SiftUp(minheap, it->v, d[it->v]);

		//for (int i = 0; i < minheap->capacity; i++)
		//	printf("%d ", minheap->pos[i]);

		//printf("\n\n");

    		}

		}

		free(u);
    }

   	printf("Parinte\tVarf\n");
    for (i = 0; i < G.nn; i++) {
    	printf("%d\t%d\t\n", parents[i], i);
    }

    dealloc_queue(minheap);

    return;

}


int main()
{
    int i,v1,v2,c;
	int V,E;
	TGraphL G;
	ATNode t, it;
	freopen ("graph.in", "r", stdin);
	scanf ("%d %d", &V, &E);
	alloc_list(&G, V);

	for (i=1; i<=E; i++)
	{
		scanf("%d %d %d", &v1, &v2, &c);
	    insert_edge_list(&G, v1,v2, c);
	}

	//for(i=0;i<G.nn;i++)
    //printf("%d : ", i);
   // for(t = G.adl[i]; t != NULL; t = t->next) {
    //printf("nod adiacent:%d cost: %d ",t->v, t->c);
    //printf("\n");
	//}
	dijkstra(G, 0);
	Prim(G);

	free_list(&G);

	return 0;
}
