#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>

/* We assume there is a defined primitive type Item. */
typedef struct {
	int prior; // Item priority
	Item data; // Item d
}ItemType;

typedef struct heap{
	long maxHeapSize; // capacity
	long size; // number of elements
	ItemType *elem; // array of elements
} PriQueue, *APriQueue;


PriQueue* makeQueue(int maxHeapSize){
	PriQueue* queue = (PriQueue*) malloc(sizeof(PriQueue));
	if (!queue) {
		printf("ALOCARE ESUATA\n");
		exit(1);
	}
	queue->maxHeapSize = maxHeapSize;
	queue->size = 0;
	queue->elem = (ItemType*) malloc(maxHeapSize * sizeof(ItemType));
	return queue;
}

int getLeftChild(int i){
	return (i<<1)+1;
}

int getRightChild(int i) {
	return (i<<1)+2;
}

int getParent(int i) {
	return (i-1)>>1;
}

ItemType getMax(APriQueue h) {
	return h->elem[0];
}

void siftUp(APriQueue h, int idx) {
	int parrent = getParent(idx);
	ItemType aux;
	while ((parrent >= 0) && (h->elem[idx].prior > h->elem[parrent].prior)) {
		aux = h->elem[idx];
		h->elem[idx] =  h->elem[parrent];
		h->elem[parrent] =  aux;

		idx = parrent;
		parrent = getParent(idx);
	}
	return;
}


void insert(PriQueue *h, ItemType x) {
	if (h->size == h->maxHeapSize) {
		h->maxHeapSize = h->size*2;
		h->elem = (ItemType*) realloc(h->elem, h->maxHeapSize*sizeof(ItemType));
	}
	h->elem[h->size] = x;
	siftUp(h, h->size);
	h->size++;
	return;
}

void siftDown(APriQueue h, int idx){
	ItemType aux;
	int indexMax, lChildIndex, rChildIndex;

	while(idx*2+1 < h->size) {

		indexMax = idx;
		lChildIndex = getLeftChild(idx);
		rChildIndex = getRightChild(idx);

		if ((lChildIndex < h->size) && (h->elem[idx].prior < h->elem[lChildIndex].prior))
			indexMax = lChildIndex;
		if ((rChildIndex < h->size) && (h->elem[indexMax].prior < h->elem[rChildIndex].prior))
			indexMax = rChildIndex;

		if (idx != indexMax) {
			aux = h->elem[indexMax];
			h->elem[indexMax] =  h->elem[idx];
			h->elem[idx] = aux;
			idx = indexMax;
		} else break;
	}
	return;
}

void removeMax(APriQueue h) {
	h->elem[0] = h->elem[--h->size];
	siftDown(h, 0);
	return;
}

void freeQueue(APriQueue h){
	free(h->elem);
	free(h);
	return;
}

#endif

