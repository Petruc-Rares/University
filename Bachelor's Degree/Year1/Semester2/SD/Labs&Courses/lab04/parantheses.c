#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 256

typedef char Item;
#include "Stack.h"

int isBalanced(const char *str, int length){

  /* TODO: Cerinta 3
   * Implementation must use a stack.
   * Do NOT forget to deallocate the memory you use.
   */
    char i = 0, c = str[i], x;
    if ((length % 2 != 0) || (str[length - 1] == '(')) {
      return 0;
    }
    Stack* stack = createStack();
    // la momentul deschiderii unei paranteze daca elementul precedent din stiva
    // este egal cu o paranteza inchisa, se testeaza paritatea
    // dimensiunii stivei, raspuns dupa care ne dam seama
    // daca parantezele sunt inchise
    /*while (c != '\0') {
      while (c == '(') {
        if (stack->head != NULL) {
          if (stack->head->elem == ')') {
            if (stack->size % 2 != 0) return 0;
          }
        }
        push(stack, c);
        i++;
        c = str[i];
      }
      while (c == ')') {
        push(stack, c);
        i++;
        c = str[i];
      }
    }
    // conditie finala ptr care un exemplu ((()) => not balanced
    if (stack->size % 2 != 0) return 0;
    destroyStack(stack);
    return 1;*/
    while (c != '\0') {
      while (c == '(') {
        push(stack, c);
        i++;
        c = str[i];
      }
      while (c == ')') { 
        // se verifica daca parantezele inchise
        // sunt in numar mai mare decat parantezele deschise 
        if (stack->head== NULL) {
          destroyStack(stack);
          return 0;
        }
        pop(stack);
        i++;
        c = str[i];
      }
    }
    // daca nu exista paranteze deschise ramase pe stive
    // inseamna ca toate au pereche
    if (stack->head == NULL) {

    destroyStack(stack);
      return 1;
    }
    
    destroyStack(stack);
    return 0;
}

int main(){
    int len;
    char buffer[MAX_INPUT_LEN];
    FILE* inputFile = fopen("input-parantheses.txt","r");
    if(inputFile == NULL) return 1;


    while(fgets(buffer, MAX_INPUT_LEN, inputFile) != NULL){
      buffer[strcspn(buffer, "\r\n")] = 0;
      len = strlen(buffer);
      if(len == 0) break;

      if(isBalanced(buffer, len))
        printf("%s ---> is balanced.\n", buffer);
      else
        printf("%s ---> not balanced.\n", buffer);
    }

    fclose(inputFile);

    return 0;
}
