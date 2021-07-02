#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long Item;
#include "BST.h"

// -----------------------------------------------------------------------------
BSTree* buildTree(Item *array, long len){
	BSTree* newTree = createTree();
	for(int i = 0; i < len; i++){
		insert(newTree,array[i]);
	}
	return newTree;
}
// -----------------------------------------------------------------------------
void preOrderPrint(BSTree* tree, BSTNode* x);
void inOrderPrint(BSTree* tree, BSTNode* x);
void postOrderPrint(BSTree* tree, BSTNode* x);
// -----------------------------------------------------------------------------


/*int isBST_(BSTree* tree, BSTNode* x, long min, long max) {
	if (x == tree->nil) return 1;

	if ((x->elem < min) || (x->elem > max)) return 0;

	return (isBST_(tree, x->l, min, x->elem) 
		&& isBST_(tree, x->r, x->elem+1l, max)); 
}

int isBST(BSTree* tree, BSTNode* x){
	long min = (minimum(tree,x))->elem;
	long max = (maximum(tree,x))->elem;

	return (isBST_(tree, x, min, max));
}
*/

int isBST_(BSTree* tree, BSTNode* node, int min, int max) {
	if (node == tree->nil) return 1;
	if ((node->elem < min) || (node->elem > max)) return 0;
	return (isBST_(tree, node->l, -10000, node->elem-1) && isBST_(tree, node->r, node->elem+1, 10000));
}
int isBST(BSTree* tree, BSTNode* x) {
	return (isBST_(tree, x->l, -10000, 10000) && isBST_(tree, x->r, -10000, 10000));
}


// -----------------------------------------------------------------------------
int main(void) {
	Item array[] = {5,3,2,4,7,6,8};

	BSTree *tree = buildTree(array, sizeof(array)/sizeof(array[0]));

	if(isBST(tree,tree->root->l))
		printf("The tree is a binary tree!\n");
	else
		printf("The tree is NOT a binary tree!\n");

	printf("Pre-Order: ");
	preOrderPrint(tree,tree->root->l);
	printf("\n");

	printf("In-Order: ");
	inOrderPrint(tree,tree->root->l);
	printf("\n");

	printf("Post-Order: ");
	postOrderPrint(tree,tree->root->l);
	printf("\n");

	destroyTree(tree);
	return 0;
}

void preOrderPrint(BSTree* tree, BSTNode* x){
	if (x != tree->nil) {
		printf("%ld ",x->elem);
		preOrderPrint(tree,x->l);
		preOrderPrint(tree,x->r);
	}
}

void inOrderPrint(BSTree* tree, BSTNode* x){
	if (x != tree->nil) {
		inOrderPrint(tree,x->l);
		printf("%ld ",x->elem);
		inOrderPrint(tree,x->r);
	}
}

void postOrderPrint(BSTree* tree, BSTNode* x){
	if (x != tree->nil) {
		postOrderPrint(tree,x->l);
		postOrderPrint(tree,x->r);
		printf("%ld ",x->elem);
	}
}


