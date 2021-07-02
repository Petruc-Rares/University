#include <stdio.h>
#include <stdlib.h>

typedef long Item;

typedef struct ListNode{
	Item elem;
	struct ListNode* next;
}ListNode;

ListNode* createList(void){
	ListNode* head = (ListNode*) malloc(sizeof(ListNode));
	return head;
}

ListNode* insertFront(ListNode* head, Item d){
	ListNode* node = (ListNode*) malloc(sizeof(ListNode));
	node->elem = d;

	node->next = head->next;
	head->next = node;

	return head;

}

void printList(ListNode* head){
	ListNode* current = head->next;
	while(current != NULL){
		printf("%ld ", current->elem);
		current = current->next;
	}
	printf("\n\n");
}


void freeList(ListNode* head){
	ListNode* current = head->next;
	while(current != NULL){
		ListNode* aux = current;
		current = current->next;
		free(aux);
	}
	free(head);
}

int main(void) {
	ListNode* list = createList();
	list = insertFront(list, 5);
	list = insertFront(list, 4);
	list = insertFront(list, 3);
	list = insertFront(list, 2);
	list = insertFront(list, 1);

	printList(list);

	freeList(list);

	return 0;
}
