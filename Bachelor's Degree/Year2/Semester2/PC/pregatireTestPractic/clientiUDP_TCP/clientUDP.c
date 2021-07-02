/*
*  	Protocoale de comunicatii:
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

#include <arpa/inet.h>
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
    fprintf(stderr, "Usage: %s ip_server port_server file\n", file);
    exit(0);
}

/*
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc, char **argv) {
    //if (argc != 4)
    //    usage(argv[0]);

    int fd;
    struct sockaddr_in to_station;
    char buf[BUFLEN];

    /*Deschidere socket*/
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    DIE(s < 0, "Socket fail");

    /* Deschidere fisier pentru citire */
  //  DIE((fd = open(argv[3], O_RDONLY)) == -1, "open file");

    /*Setare struct sockaddr_in pentru a specifica unde trimit datele*/
    to_station.sin_family = AF_INET;
    to_station.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &to_station.sin_addr);

    /*
	*  cat_timp  mai_pot_citi
	*		citeste din fisier
	*		trimite pe socket
	*/

    socklen_t len = sizeof(struct sockaddr_in);
    /*while (1) {
        int bytes_read = read(fd, buf, BUFLEN);
        printf("Bytes read from client %d\n", bytes_read);
        if (bytes_read == 0)
            break;
        sendto(s, buf, bytes_read, 0, (struct sockaddr *)&to_station, len);
    }*/
    char buffer[BUFLEN];
    while (1) {
        memset(buffer, 0, BUFLEN);
        fgets(buffer, BUFLEN - 1, stdin);

        printf("buffer client: %s\n", buffer);
        // se trimite mesaj la server
        //sendBuffer(s, buffer, BUFLEN);
        sendto(s, buffer, BUFLEN, 0, (struct sockaddr *)&to_station, len);

        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }
    }

    /*Inchidere socket*/
    close(s);

    /*Inchidere fisier*/
    //close(fd);

    return 0;
}
