#ifndef TREE_H_D
#define TREE_H_
#define MAX(x,y) ((x>y)?x:y)

#include <stdio.h>
#include <stdlib.h>

typedef int Item;

typedef struct Link
  {
    Item  elem;
    struct Link *l;
    struct Link *r;
  } TreeNode;



void Init(TreeNode **t, Item x)
{
	// MOD 1
	/*TreeNode*p = (TreeNode*)malloc(sizeof(TreeNode));
	p->elem = x;
	p->l = NULL;
	p->r = NULL;
	(*t) = p;*/

	// MOD 2
    (*t) = (TreeNode*)malloc(sizeof(TreeNode));
    if ((*t) == NULL) {
       	printf("Nu s-a putut aloca memorie pentru nodul cu valoare %d, \n", x);
       	exit(1);
    }
    (*t)->elem = x;
    (*t)->l = NULL;
    (*t)->r = NULL;
    return;
}

// implementare laborator
/*void Insert(TreeNode**t, Item x) {
	if ((*t) == NULL) {
		Init(t, x);
		return ;
	} else {
		if (x <= (*t)->elem)
			Insert(&((*t)->l), x);
		else
			Insert(&((*t)->r), x);
	}
}
*/

// implemenetare personala
void Insert(TreeNode **t, Item x)
{
	// daca arborele este vid initialziam radacina
	if ((*t) == NULL){
		Init(t, x);
		return ;
	}
	TreeNode* newNode;
	Init(&newNode, x);
	// se va parcurge arborele cu un pointer p
	TreeNode* p = *t, *prev = NULL;
	// verifica daca elementul a iesit din while pe un subarbore
	// stang sau unul drept cu ajutorul indicatorului ok
	// ok = 1 daca apartine arborelui drept
	// ok = 0 daca apartine arborelui stang
	int ok = -1; 
	while (p != NULL) {

		prev = p;

		if (p->elem < newNode->elem) {

			ok = 1;
			p = p->r;

		} else {

			ok = 0;
			p = p->l;

		}

	}

	if (ok == 1) {

		prev->r = newNode;

	} else {

		prev->l = newNode;

	}
		
	return;

	/*
	while ((p->elem < newNode->elem) || (p->elem > newNode->elem)) {
		// daca elementul nou introdus are valoarea mai mare decat
		// a nodului curent se va merge pe ramura dreapta a nodului
		// curent
		if (p->elem < newNode->elem) {
			// daca (*t)->elem e cel mai mare element din arbore
			// se va ajunge in capatul din dreapta
			ok = 1; 
			if (p->r == NULL) {
				break;
			}
			p = p->r;
		}		
		// daca elementul nou introdus are valoarea mai mica decat
		// a nodului curent se va merge pe ramura stanga a nodului
		// curent 
		else {
			// daca (*t)->elem e cel mai mic element din arbore
			// se va ajunge in capatul din stanga
			ok = 0;
			if (p->l == NULL) {
				break;
			}
			p = p->l;
		}
	}
	if (ok == 1) {
		p->r = newNode;
	} else if (ok == 0) {
		p->l = newNode;
	}*/
}

void vizit(TreeNode* t) {
	printf(" %d", t->elem);
}

void PrintPostorder(TreeNode *t)
{
	if (t == NULL) return;
	PrintPostorder(t->l);
	PrintPostorder(t->r);
	vizit(t);
}

void PrintPreorder(TreeNode *t)
{
	if(t == NULL) return;
	vizit(t);
	PrintPreorder(t->l);
	PrintPreorder(t->r);
}

void PrintInorder(TreeNode *t)
{
	if (t== NULL) return;
	PrintInorder(t->l);
	vizit(t);
	PrintInorder(t->r);
}

void vizit_(TreeNode* t) {
	free(t);
}

int anc(TreeNode* root, int x, int y){

	if (x > y) return anc(root, y, x);

	if (root == NULL) return -1;

	if (root->elem >= x || root->elem <= y) return root->elem;
	if (root->elem < x) return anc(root->r, x, y);
	return anc(root->l, x, y);

}

void Free(TreeNode **t)
{	
	if ((*t) == NULL) return;
	Free(&((*t)->l));
	Free(&((*t)->r));
	vizit_(*t);
}

// implemenetare laborator
// return Size(t->l) + 1 + Size(t->r)


int magic(TreeNode* root, int *size) {

	if(root == NULL)return 0;
	int res1 = magic(root->l, size);
	int res2 = magic(root->r, size);
	int total = res1 + res2;
	if (*size <= total) *size = total + 1;
	int result = res1;
	if (res2 > result) result = res2;
	result += 1;
	printf("%d ", result);
	return result;
}
// implementare personala
int Size(TreeNode* t)
{
	if (t == NULL) return 0;
	// radacina este primul nod

	int lResult = Size(t->l);
	int rResult = Size(t->r);
	int result = lResult + rResult;
	if (result == 0) result++;
	return result;
	//return (1 + Size(t->l) + Size(t->r));	
	/*int count = 1;
	count += Size(t->l);
	count += Size(t->r);
	return count;*/
}

int Size_interns(TreeNode* t) {

	if((t == NULL) || ((t->l == NULL) && (t->r == NULL))) return 0;
	return(1 + Size_interns(t->l) + Size_interns(t->r));

}

int Size_externs(TreeNode* t) {

	if (t == NULL) return 0;
	if ((t->l == NULL) && (t->r == NULL)) return 1;

	return (Size_externs(t->l) + Size_externs(t->r));

}

int Size_preferred(TreeNode* t) {

	int count = 0;
	if (t == NULL) return 0;
	if (t->elem >= 11) count++; 
	//else return 0;

	return(count + Size_preferred(t->l) + Size_preferred(t->r));

}
// la laborator similar cu size-ul, in rest e la fel
// ca ce ai facut

int maxDepth(TreeNode *t)
{
	
	if (t == NULL) return 0;
	/*int count = 1;
	int countr = 0;
	int countl = 0;
	countr += maxDepth(t->r);
	countl += maxDepth(t->l);
	count += MAX(countl, countr);*/
	return MAX(1 + maxDepth(t->l), 1 + maxDepth(t->r));
}
/*
typedef struct QueueNode{
	TreeNode* elem;
	struct QueueNode*next;
}QueueNode;

typedef QueueNode TcelQ;
typedef QueueNode *ACelQ;

Queue* createQueue(void){
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	if (!queue) {
		printf("Nu s-a putut aloca memorie pentru coada");
		exit(1);
	} else {
		queue->front = queue->rear = NULL;
		queue->size = 0;
	}
	return queue;
} 

int EmptyQueue(Queue* queue) {

	return queue->front  == NULL;

}

void enqueue(Queue **q, TreeNode* t){
	if (!(*q)) return;
	Item elem = t->elem;
	ACelQ lastNode = (ACelQ) malloc(sizeof(TcelQ));
	if (!lastNode) {
		printf("Nu s-a putut aloca memorie pentru nodul introdus la coada\n");
		exit(1);
	} else 
		lastNode->elem = elem;
		lastNode->next = NULL;
		if ((*q)->front == NULL) {
			(*q)->rear = (*q)->front = lastNode;
			(*q)->size = 1;
			return ;
		}
		(*q)->rear->next = lastNode;
		(*q)->rear = lastNode;
	(*q)->size++;
	return ;
}

Item front(Queue* q){
	if (!q) return 0; // default value
	if (q->front == NULL) return 0;
	return q->front->elem;
}

TreeNode* dequeue(Queue* q){
	if (!q) return;
	if (q->front == NULL) return;
	// in caz de ramane un singur nod in lista
	if (q->front->next == NULL) {
		printf("sunt aici");
		q->rear = NULL;
	}
	TreeNode* aux = q->front;
	//Item nb = q->fron->elem;
	q->front = q->front->next;
	if (q->front != NULL) {

		if (q->front->next == NULL) {

			q->rear = q->front;

		}

	}
	q->size--;
	//free(aux);
	return aux;
}


void level_printing(TreeNode* tree) {

	Queue* coada = createQueue();
	TreeNode* t = NULL;
	//printf("%d\n", tree->r->elem);

	enqueue(&coada, tree);
	//t = dequeue(coada);

	//printf("%d", (int)(coada->front == NULL));
	//printf("%d", t->elem);
	
	/*
	while (!EmptyQueue(coada)) {

		printf("1\n");
		t = dequeue(coada);
		printf("%d ", t->elem);
		printf("%d", t->r->elem);
		if(t->l != NULL) enqueue(coada, t->l);
		if(t->r != NULL) enqueue(coada, t->r); 

	}*/
/*
	return ;

}*/

#endif // LINKEDSTACK_H_INCLUDED