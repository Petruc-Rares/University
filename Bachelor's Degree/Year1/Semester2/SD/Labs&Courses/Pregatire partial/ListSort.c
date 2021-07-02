#include <stdio.h>
#include <stdlib.h>

typedef long Item;

typedef struct ListNode{
	Item elem;
	struct ListNode* next;
}ListNode;




ListNode* insertBefore(ListNode* head, Item x, Item d){
	// 1. Aloca memorie pentru un nou nod
	// 2. Depune d in nodul referit de 'node'
	// 3. Localizeaza nodul care contine x (in caz ca exista)
	// 4. Actualizeaza legaturile
	ListNode* node = (ListNode*) malloc(sizeof(ListNode));
	node->elem = d;
	node->next = NULL;

	if(head == NULL) return node;

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

	if(head == NULL) return node;

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

ListNode* createIntegerList(long a, long b){
	ListNode* list = NULL;
	if(b < a) return list;

	for(int i = b; i >= a; i--){
		list = insertBefore(list, i+1, i);
	}
	return list;
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


// Insertia unui nod intr-o lista sortata (similar cu lab 02).
void insertSorted(ListNode** head, ListNode* node) {
    // Caz special pentru detectia sfarsitului de lista partial sortata.
    if (*head == NULL || (*head)->elem >= node->elem) { 
        node->next = (*head); 
        (*head) = node; 

        return;
    } 
    

  	ListNode* current = (*head); 
    while (current->next!=NULL && current->next->elem < node->elem) { 
        current = current->next; 
    } 
    
    node->next = current->next; 
    current->next = node; 
     
}

// Varianta recursiva pentru insertia unui nod intr-o lista sortata
// void insertSorted(ListNode** head, ListNode* node) {
// 	if ((*head) == NULL || (*head)->elem > node->elem) {
// 		node->next = (*head);
// 		(*head) = node;
// 	} else {
// 		insertSorted(&((*head)->next), node);
// 	}
// }


ListNode* insertionSort(ListNode *head){
	ListNode* sorted = NULL;
	ListNode *current = head;
	while (current != NULL){
		// store next for next iteration
		ListNode* next = current->next;

		// insert current in sorted linked list
		insertSorted(&sorted, current);

		// Update current
		current = next;
	}

	return sorted;
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

	list = insertionSort(list);
	printf("insertionSort: ");
	printList(list);

	freeList(list);
	return 0;
}
