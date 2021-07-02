#include <stdio.h>
#include <stdlib.h>

typedef long Item;

typedef struct ListNode{
	Item elem;
	struct ListNode* next;
}ListNode;

ListNode* insertFront(ListNode* head, Item d){
	ListNode* node = (ListNode*) malloc(sizeof(ListNode));
	node->elem = d;
	node->next = NULL;

	if(head == NULL){
		head = node;
	}else{
		node->next = head;
		head = node;
	}

	return head;
}

ListNode* createIntegerList(long a, long b){
	ListNode* list = NULL;
	if(b < a) return list;

	for(int i = b; i >= a; i--){
		list = insertFront(list, i);
	}
	return list;
}

ListNode* insertBefore(ListNode* head, Item x, Item d){
	// 1. Aloca memorie pentru un nou nod
    // 2. Depune d in nodul referit de 'node'
	// 3. Localizeaza nodul care contine x (in caz ca exista)
	// 4. Actualizeaza legaturile
	ListNode* node = (ListNode*) malloc(sizeof(ListNode));
	node->elem = d;
	node->next = NULL;

	ListNode* prev = NULL;
	ListNode* current = head;

	while(current != NULL && current->elem != x){
		prev = current;
		current = current->next;
	}

	if(current != NULL){
		node->next = current;
		if(prev != NULL) prev->next = node;
		else head = node;
	}else{
		free(node);
	}

	return head;
}


ListNode* insertAfter(ListNode* head, Item x, Item d){
	// 1. Aloca memorie pentru un nou nod
    // 2. Depune d in nodul referit de 'node'
	// 3. Localizeaza nodul care contine x (in caz ca exista)
	// 4. Actualizeaza legaturile
	ListNode* node = (ListNode*) malloc(sizeof(ListNode));
	node->elem = d;
	node->next = NULL;

	ListNode* current = head;

	while(current != NULL && current->elem != x){
		current = current->next;
	}

	if(current != NULL){
		node->next = current->next;
		current->next = node;
	}else{
		free(node);
	}

	return head;
}

ListNode* reverseList(ListNode* head){
	ListNode* current = head;
	ListNode* next = NULL;
	ListNode* prev = NULL;

	while(current != NULL){
		next = current->next;
		current->next = prev;

		prev = current;
		current = next;
	}

	return prev;
}



void freeList(ListNode* list){
	while(list != NULL){
		ListNode* aux = list;
		list = list->next;
		free(aux);
	}
}

void printList(ListNode* list){
	while(list != NULL){
		printf("%ld ", list->elem);
		list = list->next;
	}
	printf("\n\n");
}




int main(void) {

	long a = 1, b = 9;
	ListNode* list = createIntegerList(a, b);
	printf("createIntegerList: ");
	printList(list);


	list = insertBefore(list, 3, 1000);
	printf("insertBefore: ");
	printList(list);


	list = insertAfter(list, 3, -1000);
	printf("insertAfter: ");
	printList(list);


	list = reverseList(list);
	printf("reverseList: ");
	printList(list);

	freeList(list);

	return 0;
}
