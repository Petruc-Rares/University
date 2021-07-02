#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <string.h>

typedef struct {
    unsigned char value;
    int left, right;
   	unsigned char freq;
} HuffmanNode;

#define Item int

// Including below so `Item` is defined.
#include "heap.h"

void computeFreqs(char *text, int size, int freqs[256]) {

	for (int i = 0; i < 256; i++) {
		freqs[i] = 0;
	}

	for (int i = 0; i < size; i++) {
		freqs[(int)text[i]]++;
	}

	// in cazul unui string empty
	if (size == 0) freqs[0]++;

	return;
}

HuffmanNode *makeTree(int freqs[256], int *size) {

	APriQueue queue =  makeQueue(256);
	ItemType x, aux1, aux2;
	HuffmanNode* root = NULL;
	int i;

	for (i = 0; i < 256; ++i)
	{
		if (freqs[i]) {		

			(*size)++;

			root = (HuffmanNode*) realloc(root, (*size) * sizeof(HuffmanNode));
			if (!root) {

				printf("Nu s-a putut aloca memorie pentru nod\n");
				exit(1);

			}

			root[*(size)-1].value = (unsigned char)i;
			root[*(size)-1].left = root[*(size) - 1].right = -1; // consideram -1 ca fiind frunza
			root[*(size)-1].freq = freqs[i];

			x.content = *(size)-1;
			x.prior = freqs[i];
			insert(queue, x);

		}
	}

	if (*size == 1) {

		root = (HuffmanNode*) realloc(root, (*size + 1) * sizeof(HuffmanNode));
		root[*size].left = 0;
		root[*size].right = -1;
		root[*size].value = (unsigned char) 0;

		++(*size);
	}

	// lucram pe elemente pana ramane unu singur
	while (queue->size > 1) {

		aux1 = removeMin(queue);
		aux2 = removeMin(queue);

		x.content = *size;
		x.prior = aux1.prior + aux2.prior;

		root = (HuffmanNode*)realloc(root, (*size + 1)* sizeof(HuffmanNode));
		root[*size].value = 0;
		root[*size].left = aux1.content;
		root[*size].right = aux2.content;
		root[*size].freq = aux1.prior + aux2.prior;

		(*size)++;
		insert(queue, x);
	}

	return root;
}

void makeCodes(int index, HuffmanNode *nodes, char **codes, char *code) {

	if ((nodes[index].left == -1) && (nodes[index].right == -1)) {
		codes[nodes[index].value] = strdup(code);
	}

	if (nodes[index].left != -1) {
		makeCodes(nodes[index].left, nodes, codes, strcat(code, "0"));

		code[strlen(code) - 1] = '\0';
	}

	if (nodes[index].right != -1) {
		makeCodes(nodes[index].right, nodes, codes, strcat(code, "1"));
		
		code[strlen(code) - 1] = '\0';

	}


}

char *compress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {
	if (textSize == 0) return "0";

	char* compressed = (char*) malloc(20000 * sizeof(char));
	compressed[0] = '\0';

	if (!textSize) {
		return compressed;
	}

	char code[100] = {0};
	char **codes = (char**) calloc(256, sizeof(char*));

	makeCodes(numNodes -1, nodes, codes, code);

	printf("%s", codes[(unsigned char)'o']);

	int i;

	for (i = 0; i < textSize; i++) {
		strcat(compressed, codes[(unsigned int) text[i]]);
	}

	return compressed;

}

char *decompress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {
	if (textSize == 0) return "\0";
	int rootNode = numNodes - 1;

	//printf("%s\n", text);

	char* decompressed = (char*)malloc(20000*sizeof(char));
	decompressed[0] = '\0';

	int k = 0;
	int idx = rootNode;
	char symbol;
	for (int i = 0; i < textSize; i++) {
		symbol = text[i];

		if (symbol == '0') {
			idx = nodes[idx].left;
		} else {
			idx = nodes[idx].right;
		}

		if ((nodes[idx].left == -1) && (nodes[idx].right == -1)) {
			decompressed[k++] = nodes[idx].value;
			idx = rootNode;
		}
	}

	decompressed[k] = '\0';
	return decompressed;

}

#endif

