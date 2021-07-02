/*
*  	Protocoale de comunicatii:
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helpers.h"

void usage(char *file) {
    fprintf(stderr, "Usage: %s server_port file\n", file);
    exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc, char **argv) {
    int fd;

    if (argc != 3)
        usage(argv[0]);

    struct sockaddr_in my_sockaddr, from_station;
    char buf[BUFLEN];

    /*Deschidere socket*/
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    DIE(s < 0, "Socket fail");

    /*Setare struct sockaddr_in pentru a asculta pe portul respectiv */
    from_station.sin_family = AF_INET;
    from_station.sin_port = htons(atoi(argv[1]));
    from_station.sin_addr.s_addr = INADDR_ANY;

    /* Legare proprietati de socket */
    int rs = bind(s, (struct sockaddr *)&from_station, sizeof(my_sockaddr));
    DIE(rs < 0, "Bind fail");

    /* Deschidere fisier pentru scriere */
    DIE((fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1, "open file");

    /*
	*  cat_timp  mai_pot_citi
	*
	*		pune in fisier
	*/

    socklen_t len = sizeof(struct sockaddr_in);
    while (1) {
        int bytes_read = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&from_station, &len);
        printf("Bytes received by server %d\n", bytes_read);
        if (bytes_read <= 0)
            break;
        write(fd, buf, bytes_read);
    }

    /*Inchidere socket*/
    close(s);
    /*Inchidere fisier*/
    close(fd);
    return 0;
}
