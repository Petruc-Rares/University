#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"


void mirror(TreeNode **t) {
    if (((*t) == NULL) || (((*t)->l == NULL) && ((*t)->r == NULL))) return;  
    TreeNode *p = *t;
    TreeNode *aux = NULL;
    aux = p->l;
    p->l = p->r;
    p->r = aux;
    mirror(&(p->l));
    mirror(&(p->r));
    return;
}

// implemenetare lab
/*if (*t == NULL) return;
mirror(&(*t)->l);
mirror(&(*t)->r);
interschimbare*/

// implementare personala dubioasa, dar merge.. putin diferit
// ineficient pentru ca se apeleaza Size..
// cum as putea da exit intr-un anume moment in care vad ca doua valori difera (NU POTI)
// daca functia returneaza 0 inseamna ca nu coincid arborii
// daca functia returneaza orice val diferita de 0 inseamna ca arborii coincid
/*int sameTree(TreeNode *t1, TreeNode *t2) {
  if (((t1 == NULL) || (t2 == NULL)) || (t1->elem != t2->elem)) return 0;
  int noCellsT1, noCellsT2, noCellsSame = 1;
  noCellsT1 = Size(t1);
  noCellsT2 = Size(t2);
  if (noCellsT1 != noCellsT2) return 0;
  noCellsSame += sameTree(t1->l, t2->l);
  noCellsSame += sameTree(t1->r, t2->r);
  if (noCellsSame != noCellsT1) return 0;
  return noCellsSame;
}*/

// implementare personala
int sameTree(TreeNode *t1, TreeNode *t2) {
  if ((t1 == NULL) && (t2 == NULL)) return 1;
  if ((t1 == NULL) && (t2 != NULL) || (t1 !=NULL) && (t2 == NULL)) return 0;
  if (t1->elem != t2->elem) return 0;
  return ((sameTree(t1->l, t2->l)) && (sameTree(t1->r, t2->r)));
}

// implementare lab
/*if (t1 == NULL && t2 == NULL) return 1
if(t1!=NULL && t2 != NULL)
    return {
        t1->elem == t2->elem &&
        sameTree(t1->l, t2->l) &&
        sameTree(t1->r, t2->r)
} else return 0;
 */

int main()
{


	TreeNode*t = NULL;
	Insert(&t, 8); Insert(&t, 9); Insert(&t, 11); Insert(&t, 10);
	Insert(&t, 3); Insert(&t, 1); Insert(&t, 6); Insert(&t, 5);
	Insert(&t, 7);
	printf("%d", anc(t, 1, 7));
	printf(" %d", anc(t, 9, 10));
	/*Insert(&t, 11);
	Insert(&t, 8); Insert(&t, 7); Insert(&t, 4); Insert(&t, 5);
	*/
/*

	    printf("Size should be: 12 \n");
    printf("Size = %d \n \n", Size(t));
    printf("Size_interns should be: 8 \n");
    printf("Size_interns = %d \n \n", Size_interns(t));
        printf("Size_interns should be: 4 \n");
    printf("Size_interns = %d \n \n", Size_externs(t));
            printf("Size_preferred should be: 7 \n");
    printf("Size_preferred = %d \n \n", Size_preferred(t));

    printf("MaxDepth should be: 5 \n");
    printf("MaxDepth = %d \n \n", maxDepth(t));*/

    /*
    printf("Every level printing: ");
    level_printing(t);*/

	/*
    TreeNode *t1=NULL, *t2=NULL;
    Insert(&t1, 1); Insert(&t1, 2);
    Insert(&t1, 3); Insert(&t1, 4);

    
    Insert(&t2, 1); Insert(&t2, 2);
    Insert(&t2, 3); Insert(&t2, 5);

    printf("Preorder should be: 1 2 3 4 \nPreorder = ");
    PrintPreorder(t1);
    printf("\n\n");

    printf("Inorder should be: 1 2 3 4 \nInorder = ");
    PrintInorder(t1);
    printf("\n\n");

    printf("Postorder should be: 4 3 2 1 \nPostorder = ");
    PrintPostorder(t1);
    printf("\n\n");
    
    printf("Size should be: 4 \n");
    printf("Size = %d \n \n", Size(t1));


    printf("MaxDepth should be: 4 \n");
    printf("MaxDepth = %d \n \n", maxDepth(t1));

    printf("Same tree should be: 0 \n");
    printf("Same tree = %d \n\n", sameTree(t1,t2));


    printf("Before swap (mirror). Inorder should be: 1 2 3 5 \n"); PrintInorder(t2);
    printf("\n");
    mirror(&t2);
    printf("After swap (mirror). Inorder should be: 5 3 2 1 \n"); PrintInorder(t2);
    printf("\n");

    Free(&t1);
    Free(&t2);
*/ Free(&t);
    return 0;
}
