#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Maximum number of characters that can be read
 * in one go from the keyboard
 */
#define MAX_INPUT_LEN 255

/**
 *  Generic Item type definition
 */
typedef char Item;

/**
 * The Double linked list interface
 */
#include "DoubleLinkedList.h"

/**
 * Utility functions for character lists
 */
void printList(List *list){
	ListNode* iterator = list->first;
	printf("[");
	while(iterator != list->last){
		printf("%c, ", iterator->elem);
		iterator = iterator->next;
	}
	printf("%c]",list->last->elem);
}

void printListReverse(List* list){
	ListNode* iterator = list->last;
	printf("[");
	while(iterator != list->last){
		printf("%c ", iterator->elem);
		iterator = iterator->prev;
	}
	printf("%c]",list->last->elem);
}




/* Plaindrome computation function */
int isPalindrome(List* list){
	// TODO: Cerinta 2
	if (list == NULL) return 1;
	ListNode *p1, *p2;
	p1 = list->first;
	p2 = list->last;
	// cat timp pointerii nu se invecineaza (cazul par)
	// si cat timp pointerii nu se suprapun (cazul impar)
	while ((p1->prev != p2) && (p1 != p2)) {
		if (p1->elem != p2->elem) return 0;
		p1 = p1->next;
		p2 = p2->prev;
	}
	// in urma ciclului
	// vor ramane 2 elemente ce nu au fost comparate (cazul par)
	// va ramane 1 element la cazul impar
	//if (p1->elem != p2->elem) return 0;
	return 1;
}

/* Main Program */
int main(void) {

	int len;
	int inputCount = 0;
	char inputCharacters[256];
	FILE* inputFile = fopen("input","r");



	while(fgets(inputCharacters, MAX_INPUT_LEN, inputFile) != NULL){
			// Remove trailing CR/LF (i.e. '\r', '\n')
			inputCharacters[strcspn(inputCharacters, "\r\n")] = 0;
			len = strlen(inputCharacters);
			if(len == 0) break;

			List *list = createList();
			for(int i = 0; i < len; i++) insertAt(list, inputCharacters[i],i);


			printf("Input%d: ", ++inputCount);
			printf("\"%s\" --- ", inputCharacters);
			printf("List: ");
			printList(list);

			if(isPalindrome(list)) printf(" is a palindrome.");
			else printf(" is NOT a palindorme.");
			printf("\n");

			destroyList(list);
	}

	fclose(inputFile);
	return 0;
}



