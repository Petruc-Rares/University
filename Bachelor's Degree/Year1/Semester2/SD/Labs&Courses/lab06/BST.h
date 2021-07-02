#ifndef BST_H_
#define BST_H_

#include <stdio.h>
#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just assume
  some type T was previously defined.
 */

// -----------------------------------------------------------------------------

/*
 * !!!!ATENTION!!!!
 * 1. This implementation uses two sentinels (dummy nodes) root and nil.
 * 2. The convetion is that we always root the tree at root->l (the left of the root sentinel)
 */

typedef struct BSTNode{
	Item elem;
	struct BSTNode *p; // parent - not really needed, but simplifies implementation.
	struct BSTNode *l; // left child.
	struct BSTNode *r; // right child.
}BSTNode;

typedef struct BSTree{
	long size; // This inintially is 0, and is updated by insert/delete
	BSTNode *root; // The sentinel root (a sentinel not the actual root). The actual root node is root->l !
	BSTNode *nil; // The nil sentinel to avoid checks for NULL
}BSTree;

// TODO: Cerinta 1 si 2

BSTree* createTree(void) {
	BSTree* tree = (BSTree*) malloc(sizeof(BSTree));
	tree->size = 0;
	tree->root = (BSTNode*) malloc(sizeof(BSTNode));
	tree->nil = (BSTNode*) malloc(sizeof(BSTNode));	
	tree->root->l = tree->root->r = tree->root->p = tree->nil;
	tree->nil->l = tree->nil->r = tree->nil->p = tree->nil;
	return tree;
}

int isEmpty(BSTree* tree) {
	if (tree->root->l != tree->nil) return 0;
	return 1;
}

void insert(BSTree* tree, Item value) {
	BSTNode* current, *parrent;
	parrent = tree->root;
	current = tree->root->l;
	while (current != tree->nil) {
		parrent = current;
		if (current->elem < value)
			current = current->r;
		else if (current->elem > value)
			current = current->l;
		else return;
	}
	BSTNode* newNode = (BSTNode*) malloc(sizeof(BSTNode));
	newNode->elem = value;
	newNode->p = parrent;
	newNode->l = newNode->r = tree->nil;
	if ((parrent == tree->root) || (parrent->elem > value))
		parrent->l = newNode;
	else 
		parrent->r = newNode;
	tree->size++;
	return;
}

BSTNode* search(BSTree* tree, Item value) {
	BSTNode* current = tree->root->l;
	while (current != tree->nil) {
		if (current->elem < value) 
			current = current->r;
		else if (current->elem > value)
			current = current->l;
		else return current;
	}
	return NULL;
}

BSTNode* minimum(BSTree* tree, BSTNode* node) {
	while (node->l != tree->nil)
		node = node->l;
	return node;
}

BSTNode* maximum(BSTree* tree, BSTNode* node) {
	while (node->r != tree->nil)
		node = node->r;
	return node;
}

BSTNode* successor(BSTree* tree, BSTNode* node) {
	if (node == NULL) return NULL;
	if (node->r != tree->nil)
		return minimum(tree, node->r);

	BSTNode* parrent = node->p;

	while ((parrent != tree->root) && (node == parrent->r)) {
		node = parrent;
		parrent = parrent->p;
	}
	if (parrent == tree->root) return NULL;
	return parrent;
}

BSTNode* predecessor(BSTree* tree, BSTNode* node) {
	if (node == NULL) return NULL;
	if (node->l != tree->nil)
		return maximum(tree, node->l);

	BSTNode* parrent = node->p;

	while ((parrent != tree->root) && (node == parrent->l)) {
		node = parrent;
		parrent = parrent->p;
	}
	if (parrent == tree->root) return NULL;
	return parrent;
}

void destroyTree_(BSTree* tree, BSTNode* node) {
	if (node == tree->nil) return;

	destroyTree_(tree, node->l);
	destroyTree_(tree, node->r);

	//printf("%lu\n", node->elem);

	free(node);
	return;
}

void destroyTree(BSTree* tree) {
	destroyTree_(tree, tree->root->l);
	free(tree->root);
	free(tree->nil);
	free(tree);
	return;
}

void helperTestLevel(BSTree* tree, BSTNode* curNode, int level,
					 int* contor, int curLevel, int* array) {
	if (curNode == tree->nil) return;
	if (curLevel == level) {
		array[*(contor)] =  curNode->elem;
		(*contor)++;
	} else if (curLevel > level) return;
	helperTestLevel(tree, curNode->l, level, contor, curLevel+1, array);
	helperTestLevel(tree, curNode->r, level, contor, curLevel+1, array);
	return;
}

void testLevel(BSTree *tree, int level, int *contor, int *array) {
	helperTestLevel(tree, tree->root->l, level, contor, 1, array);
	for (int i = 0; i < *contor; i++) 
		printf("%d ", array[i]);
	printf("\n%d\n", *contor);
	return;
}

#endif /* BST_H_ */

