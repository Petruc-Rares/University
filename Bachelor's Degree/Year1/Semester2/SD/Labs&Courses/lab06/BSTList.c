/*
 * BSTList.c
 *
 *  Created on: Apr 2, 2016
 *      Author: dan
 */

typedef long Item;
#include "BST.h"
typedef BSTNode* Item2;
#include "List.h"
#include "Queue.h"


BSTree* buildTree(Item *array, long len){
	BSTree* newTree = createTree();
	for(int i = 0; i < len; i++){
		insert(newTree,array[i]);
	}
	return newTree;
}

// -----------------------------------------------------------------------------

/*
List* bstToList(BSTree* tree){ 
	List* list = createList();

	Queue* q;
	initQueue(&q);

	BSTNode* node = tree->root->l;
	enqueue(q, node);

	while (isEmptyQueue(q) == 0) {
		node = front(q);
		listPushBack(list, node);
		dequeue(q);

		if (node->l != tree->nil)
			enqueue(q, node->l);

		if (node->r != tree->nil)
			enqueue(q, node->r);
	}
	destroyQueue(q);

	return list;
}
*/

List* bstToList(BSTree* tree){ 
	List* newList = createList();

	Queue* queue = NULL;
	initQueue(&queue);

	BSTNode* listElem = tree->root->l;
	enqueue(queue, listElem);

	while (!isEmptyQueue(queue)) {
		listElem = front(queue);
		dequeue(queue);
		listPushBack(newList, listElem);

		if (listElem->l != tree->nil) {
			enqueue(queue, listElem->l);
		}

		if (listElem->r != tree->nil) {
			enqueue(queue, listElem->r);
		}
	}
	destroyQueue(queue);

	return newList;

}

void printList(List* list){
	for (ListNode* it = list->head->next; it != list->head; it = it->next) {
		printf ("%ld ", it->elem->elem);
	}
	printf("\n");
}
// -----------------------------------------------------------------------------

int main(void){
	Item array[] = {5,3,2,4,7,6,8};
	BSTree *tree = buildTree(array, sizeof(array)/sizeof(Item));	

	List* list = bstToList(tree);
	printList(list);

	destroyList(list);

	destroyTree(tree);

	return 0;
}