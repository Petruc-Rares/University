#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void fatal(char *mesaj_eroare) {
	perror(mesaj_eroare);
	exit(0);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Ar trebui sa rulezi de forma ./cat nume_fisier\n");
		exit(0);
	}

	int fisier_sursa = open(argv[1], O_RDONLY);
	char buf[1];

	if (fisier_sursa < 0) {
		fatal("Fisierul nu poate fi deschis");
	}

	lseek(fisier_sursa, 0, SEEK_SET);

	char c;

	while ((c = read(fisier_sursa, buf, 1)) != 0) {
        if (c < 0) {
            fatal("Eroare la citire");
		}
		c = write(1, buf, 1);
        if (c < 0) {
            fatal("Eroare la scriere");
		}
	}

	close(fisier_sursa);
}
