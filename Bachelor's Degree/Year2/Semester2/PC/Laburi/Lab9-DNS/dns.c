// Protocoale de comunicatii
// Laborator 9 - DNS
// dns.c

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char* name)
{
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char* name)
{
	int ret;
	struct addrinfo hints, *result, *p;

	// TODO: set hints
	memset(&hints, 0, sizeof(hints));
	// acceptam fie IPv4, fie IPv6
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_CANONNAME;

	// TODO: get addresses
	ret = getaddrinfo(name, NULL, &hints, &result);
	if (ret < 0) {
		perror("failed to return socket address");
		exit(-1);
	}

	// TODO: iterate through addresses and print them
	for (p = result; p != NULL; p = p->ai_next) {
		// daca e IPv4
		if (p->ai_family == AF_INET) {
			char ip[INET_ADDRSTRLEN];
			struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;

			if (inet_ntop(p->ai_family, &(addr->sin_addr), ip, INET_ADDRSTRLEN) == NULL) continue;

			printf("IP: %s, Port: %hu, Protocol: %d, Socket type: %d, Cannon name: %s\n",
				ip, ntohs(addr->sin_port), p->ai_protocol, p->ai_socktype, p->ai_canonname);
		} else if (p->ai_family == AF_INET6) {
			char ip[INET6_ADDRSTRLEN];
			struct sockaddr_in6 *addr = (struct sockaddr_in6 *)p->ai_addr;
			
			if (inet_ntop(p->ai_family, &(addr->sin6_addr), ip, INET6_ADDRSTRLEN) == NULL) continue;

			printf("IP: %s, Port: %hu, Protocol: %d, Socket type: %d, Cannon name: %s\n",
					ip, ntohs(addr->sin6_port), p->ai_protocol, p->ai_socktype, p->ai_canonname);			
		}
	}

	// TODO: free allocated data
	freeaddrinfo(result);
}

// Receives an address and prints the associated name and service
void get_name(char* ip)
{
	int ret;
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

	// TODO: fill in address data
	// presupunem IPv4
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = 80;

	ret = inet_aton(ip, &addr.sin_addr);
	if (ret < 0) {
		perror("failed to convert");
		exit(-1);
	}

	// TODO: get name and service
	getnameinfo((struct sockaddr *)&addr, sizeof(addr), host,
				sizeof(host), service, sizeof(service), 0);

	// TODO: print name and service
	printf("host: %s\n", host);
	printf("service: %s\n", service);
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0) {
		get_ip(argv[2]);
	} else if (strncmp(argv[1], "-a", 2) == 0) {
		get_name(argv[2]);
	} else {
		return usage(argv[0]);
	}

	return 0;
}
