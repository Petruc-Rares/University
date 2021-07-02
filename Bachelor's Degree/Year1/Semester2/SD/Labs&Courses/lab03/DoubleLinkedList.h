#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------
/**
 *  Linked list representation -- R2 Curs2 (Slide 8)
 */
typedef struct ListNode{
	Item elem; // Stored node value
	struct ListNode* next; // link to next node
	struct ListNode* prev; // link to prev node
} ListNode;

/**
 *  Double linked list representation -- desen Curs 3 (Slide 19)
 */
typedef struct List{
	ListNode* first; // link to the first node
	ListNode* last; // link to the last node
}List;
// -----------------------------------------------------------------------------

/**
 * Create a new (empty)  list  -- Silde-urile 13/14 Curs 2
 *  [input]: None
 *  [output]: List*
 */
List* createList(void){
	// TODO: Cerinta 1a
	List* lista = (List*) malloc(sizeof(List));
	if (!lista) {
		printf("Nu s-a putut realiza alocarea pentru lista");
		exit(1);
	}
	lista->first = NULL;
	lista->last = NULL;
	return lista;
}
// -----------------------------------------------------------------------------


/**
 * Determines if the list is empty
 *  [input]: List*
 *  [output]: 1 - empty/ 0 - not empty
 */
int isEmpty(List *list){
	// TODO: Cerinta 1b
	if (list->first == NULL) {
		return 1;
	} else {
		return 0;
	}
}
// -----------------------------------------------------------------------------


/**
 * Determines if a list contains a specified element
 *  [input]: List*, Item
 *  [output]: int (1 - contains/ 0 - not contains)
 */
int contains(List *list, Item elem){
	// TDOO: Cerinta 1c
	ListNode* p = list->first;
	if (p == NULL) return 0;
	while (p != NULL) {
		if (p->elem == elem) return 1;
		p = p->next;
	}
	return 0;
	/*while ((p != NULL) && (p->elem != elem)) {
		p = p->next;
	}
	if (p == NULL) {
		return 0;
	} else {
		return 1;
	}
	return 0;*/
}
// -----------------------------------------------------------------------------


/**
 * Insert a new element in the list at the specified position.
 * Note: Position numbering starts with the position at index zero
 *  [input]: List*, Item, int
 *  [output]: void
 */
void insertAt(List* list, Item elem, int pos){

	// Guard against young player errors
	if(list == NULL) return;
	if (pos < 0) return ;

	// TODO: Cerinta 1d
	ListNode* current = list->first, *p = list->first, *prev = NULL;	// daca lista e goala iar pozitia specificata de
	// introducere a celulei este diferita de 0, nu
	// introducem nimic (indexarea incepe de la 0)
	if ((p == NULL) && (pos != 0)) {
		return ;
	}
	// daca lista e goala iar pozitia specificata de
	// introducere a celulei este 0, atunci introducem
	// prima celula
	if ((p == NULL) && (pos == 0)) {
		ListNode* newElem = (ListNode*) malloc(sizeof(ListNode));
		newElem->elem = elem;
		newElem->next = NULL;
		newElem->prev = NULL;
		list->first = newElem;
		list->last = newElem;
		return ;
	}
	int auxpos = pos, contor = 0;
	while ((p != NULL) && (pos--)) {
		prev = p;
		p = p->next;
	}
	while (current != NULL) {
		current = current->next;	
		contor++;
	}
	// nu putem avea celule ale listei in aer fara prev sau next
	if (pos > 0) return;
	ListNode* newElem = (ListNode*) malloc(sizeof(ListNode));
	newElem->elem = elem;
	newElem->next = NULL;
	newElem->prev = NULL;
	// se verifica daca se doreste a insera la inceputul listei
	if (auxpos == 0) {
		newElem->next = list->first;
		//newElem->prev = NULL;
		list->first->prev = newElem;
		list->first = newElem;
		return ;
	// daca se doreste a insera la sfarsitul listei
	} else if (contor == auxpos) {
		list->last->next = newElem;
		//newElem->next = NULL;
		newElem->prev = list->last;
		list->last = newElem;
		return ;
	// caz general
	} else {
		newElem->next = p;
		newElem->prev = prev;
		prev->next = newElem;
		p->prev = newElem;
		return ;
	}
}
/*while (pos&&iterator!=NULL)*/
// -----------------------------------------------------------------------------


/**
 * Delete the first element instance form a list.
 *  [input]: List*, Item
 *  [output]: void
 */
void deleteOnce(List *list, Item elem){
	// Guard against young player errors
	if(list == NULL) return;
	if (contains(list, elem) == 0) {
		return ;
	}
	ListNode *prev = NULL, *p = list->first;
	while ((p != NULL) && (p->elem != elem)) {
		prev = p;
		p = p->next;
	}
	if (p == NULL) return ;
	// daca elementul care este cautat se afla in prima celula
	if (prev == NULL) {
		list->first = list->first->next;
		// daca se elimina si ultima celula
		if (list->first == NULL) {list->last = NULL;free(p); return ;}
		// daca celula ramane singura
		if (list->first->next == NULL) {
			list->last = list->first;
		}
		list->first->prev = NULL;
		free(p);
		return ;
	}
	// daca elementul care este cautat se afla pe ultima celula
	 else if (p->next == NULL) {
		list->last = list->last->prev;
		list->last->next = NULL;
		free(p);
		return;
	} else {
		prev->next = p->next;
		p->next->prev = prev;
		free(p);
		return ;
	}
	//TODO: Cerinta 1e
}
// -----------------------------------------------------------------------------


/**
 * Compute list length
 *  [input]: List*
 *  [output]: int
 */
int length(List *list){
	// Guard against young player errors
	if(list == NULL) return 0;

	// TODO: Cerinta 1f
	ListNode *p = list->first;
	int contor = 0;
	while (p != NULL) {
		p = p->next;
		contor++;
	}
	return contor;
}
// -----------------------------------------------------------------------------



/**
 * Destroy a list.
 *  [input]: List*
 *  [output]: void
 */
void destroyList(List* list){
	// Guard against young player errors
	if(list == NULL) return;
	ListNode *p = list->first, *aux = NULL;
	while (p) {
		aux = p;
		p = p->next;
		free(aux);
	}
	free(list);
	return ;
	//TODO: Cerinta 1g
}
// -----------------------------------------------------------------------------


#endif //_DOUBLE_LINKED_LIST_H_
