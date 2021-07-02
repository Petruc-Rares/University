#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <string.h>

typedef struct {
    unsigned char value;
    unsigned char freq;
    int left, right;
} HuffmanNode;

#define Item int

// Including below so `Item` is defined.
#include "heap.h"

void computeFreqs(char *text, int size, int freqs[256]) {
	int i;

	for (i = 0; i < 256; i++)
		freqs[i] = 0;

	if (size == 0) return;

	for (i = 0; i < size; i++) 
		freqs[(unsigned char)text[i]]++;

	return;
}

HuffmanNode *makeTree(int freqs[256], int *size) {
	HuffmanNode* nodes = NULL;
	int i;
	ItemType x, aux1, aux2;
	APriQueue queue = makeQueue(256);

	for (i = 0; i < 256; i++) {
		if (freqs[i]) {
			nodes = (HuffmanNode*) realloc(nodes, ((*size)+1)*sizeof(HuffmanNode));

			nodes[*size].value = i;
			nodes[*size].freq = freqs[i];
			nodes[*size].left = nodes[*size].right = -1; // this is a leaf

			x.prior = freqs[i];
			x.content = *size;

			insert(queue, x);

			(*size)++;
		}
	}

	if (*size == 1) {
		nodes = (HuffmanNode*) realloc(nodes, 2*sizeof(HuffmanNode));
		nodes[*size].value = 0;
		nodes[*size].left = 0;
		nodes[*size].right = -1;
		nodes[*size].freq = nodes[0].freq;

		++(*size);
	}

	while (queue->size > 1) {
		aux1 = removeMin(queue);
		aux2 = removeMin(queue);

		nodes = (HuffmanNode*) realloc(nodes, ((*size)+1)*sizeof(HuffmanNode));
		nodes[*(size)].value = 0; // nod intern
		nodes[*(size)].left = aux1.content;
		nodes[*(size)].right = aux2.content;
		nodes[*(size)].freq = aux1.prior+aux2.prior;

		x.prior = aux1.prior + aux2.prior;
		x.content = *size; // nod intern

		insert(queue, x);

		(*size)++;
	}
	return nodes; 
}

void makeCodes(int index, HuffmanNode *nodes, char **codes, char *code) {
	if ((nodes[index].left == -1) && (nodes[index].right == -1)) {
		codes[nodes[index].value] = strdup(code);
		return;
	}
	if (nodes[index].left != -1) {
		makeCodes(nodes[index].left, nodes, codes, strcat(code, "0"));
		code[strlen(code)-1] = '\0';
	}

	if (nodes[index].right != -1) {
		makeCodes(nodes[index].right, nodes, codes, strcat(code, "1"));
		code[strlen(code)-1] = '\0';
	}
	return;
}

char *compress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {

	if (nodes == NULL) return "ANA";
	int i;
	char* compressed = (char*) malloc(1);
	char* code = (char*) malloc(40);
	code[0] = '\0';
	char** codes = (char**) calloc(256, sizeof(char*));

	makeCodes(numNodes-1, nodes, codes, code);

	for (i = 0; i < textSize; i++) {
		strncat(compressed, codes[(unsigned char)text[i]], 10);
	}

	return compressed;
}

char *decompress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {
	if (nodes == NULL) return "";
	char* decompressed = (char*) malloc(20000);
	decompressed[0] = '\0';
	int i, index = numNodes-1, k = 0;

	for (i = 0; i < textSize; i++) {

	    if (text[i] == '0')
			index = nodes[index].left;

		else if (text[i] == '1')
			index = nodes[index].right;

		if ((nodes[index].left == -1) && (nodes[index].right == -1)) {
			decompressed[k++]=nodes[index].value;
			index = numNodes-1;
		}

	}

	decompressed[k] = '\0';
	return decompressed;

}

#endif

