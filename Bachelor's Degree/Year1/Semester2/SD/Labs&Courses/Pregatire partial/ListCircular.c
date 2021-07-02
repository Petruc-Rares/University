#include <stdio.h>
#include <stdlib.h>


typedef long Item;

typedef struct ListNode{
	Item elem;
	struct ListNode* next;
}ListNode;

ListNode* insertAfter(ListNode* head, Item x, Item d){
	ListNode* node = (ListNode*) malloc(sizeof(ListNode));
	node->elem = d;
	node->next = node;

	if(head == NULL) return node;

	ListNode* current = head;
	while(current->next != head && current->elem != x){
		current = current->next;
	}

	if(current->elem != x)
		return head;

	node->next = current->next;
	current->next = node;

	return head;
}


ListNode* createIntegerList(int N){
	ListNode* list = NULL;

	for(int i = 1; i <= N; i++){
		list = insertAfter(list, i-1, i);
	}
	return list;
}

ListNode* jelim(ListNode* head, int M){
	ListNode* prev = NULL;
	ListNode* current = head;
	for(int i = 1; i < M; i++){
		prev = current;
		current = current->next;
	}

	prev->next = current->next;
	free(current);

	return prev->next;
}

void freeList(ListNode* head){
	ListNode* current = head;
	while(current->next != head){
		ListNode* aux = head;
		head = head->next;
		free(aux);
	}
}

void printList(ListNode* head){
	ListNode* current = head;
	while(current->next != head){
		printf("%ld ", current->elem);
		current = current->next;
	}

	printf("%ld ", current->elem);
	printf("\n\n");
}



int main(void) {

	int i = 0, n = 9, m = 5;
	ListNode* list = createIntegerList(n);

	printf("createIntegerList: ");
	printList(list);


	while(list != list->next){
		list = jelim(list, m);
		printf("Iteratia %d:\t", i++);
		printList(list);
	}

	return 0;
}
