#include <stdio.h>
#include "helpers.h"

// citeste dintr-un fisier ce contine conturi sub forma de:
// username1 parola1
// username2 parola2
int main() {
    FILE *accts;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    accts = fopen("conturi.txt", "r");
    while (fscanf(accts, "%s %s", username, password) > 0) {
       printf("%s %s\n", username, password); 
    }
}