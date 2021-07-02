#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void writeReverse(FILE *fisier_sursa) {
	size_t bufsize = 30;
	char *buffer = (char *) malloc(bufsize * sizeof(char));

    size_t characters = getline(&buffer, &bufsize, fisier_sursa);
    if (characters == -1) return;
	writeReverse(fisier_sursa);
	printf("%s", buffer);
}


void fatal(char *mesaj_eroare) {
	perror(mesaj_eroare);
	exit(0);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Ar trebui sa rulezi de forma ./tac nume_fisier\n");
		exit(0);
	}

	FILE *fisier_sursa = fopen(argv[1], "r");

	if (fisier_sursa < 0) {
		fatal("Fisierul nu poate fi deschis");
	}

	fseek(fisier_sursa, 0, SEEK_SET);

	writeReverse(fisier_sursa);

	fclose(fisier_sursa);

	return 0;
}