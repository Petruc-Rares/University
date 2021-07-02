#include <queue.h>
#include "skel.h"
#include "trie.h"
#include "commonStructures.h"

#define MAXSIZE 64265

int arp_table_len;
struct arp_entry arp_table[MAXSIZE];

TrieNode *root;

/*
 Returns a pointer (eg. &arp_table[i]) to the best matching ARP entry.
 for the given dest_ip or NULL if there is no matching entry.
*/
struct arp_entry *get_arp_entry(__u32 ip) {
    for (int i = 0; i < arp_table_len; i++) {
		if (arp_table[i].ip == ip) {
			return &arp_table[i];
		}
	}
    return NULL;
}

void readRtable(FILE *input_file) {
    char line[100];
    char prefix[15];
    char next_hop[15];
    char mask[15];
    int interface_no;

    while (fgets (line, sizeof(line), input_file) != NULL) {

        if (line[0] == '\0') {
            printf ("Blank line found, don't know what to do\n");
            exit(1);
        }

        if (line[strlen (line)-1] != '\n') {
            printf ("Line too long: %s", line);
            exit(1);
        }

        line[strlen(line)-1] = '\0';

        // get the values on the current line, which should be in number of 4
        if (sscanf (line, "%s %s %s %d", prefix, next_hop, mask, &interface_no) != 4) {
            printf ("We have less than 4 or more than 4 values on line: %s\n", line);
            exit(1);
        }

		struct route_table_entry *r = malloc(sizeof(struct route_table_entry));
		r->prefix = inet_addr(prefix);
		r->next_hop = inet_addr(next_hop);
		r->mask = inet_addr(mask);
		r->interface = interface_no;

		root = insert_trie(root, r);
    }

    return ;
}

uint16_t sum_no_overflow(uint16_t elem1, uint16_t elem2) {
	// check for overflow
	if (elem1 + elem2 < elem1) {
		return elem1 + elem2 + 1;
	}
	return elem1 + elem2;
}

uint16_t ip_checksum_new(uint16_t prevCheck, uint16_t ttl) {
	uint8_t m = ttl;	// old value
	uint8_t m_ = ttl - 1;	// new value
	uint16_t hc_ = ~prevCheck;

	// daca e overflow la tot mesajul 
	uint16_t sum1 = sum_no_overflow(hc_, ~m);
	uint16_t sum2 = sum_no_overflow(sum1, m_);
	return ~sum2;
}

int main(int argc, char *argv[])
{
	packet m;
	int rc;

	queue q = queue_create();
	init(argc - 2, argv + 2);
    FILE *input_file;

    // Open the input file.
    input_file = fopen (argv[1], "r");
    if (input_file == NULL) {
        printf ("Cannot open file\n");
        return 1;
    }

	root = make_trienode(NULL);
    readRtable(input_file);

    fclose (input_file);

	while (1) {
		/* 1. Primeste un pachet de la oricare din interfetele adiacente. */
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");

		struct iphdr *ip_hdr = ( struct iphdr *)(m.payload + sizeof(struct ether_header));
		struct ether_header *eth_hdr = (struct ether_header *)m.payload;

		// get the interface the packet was sent through
		int interface_received = m.interface;
		// get the ip corresponding to the interface
		uint32_t ip_interface_received =  inet_addr(get_interface_ip(interface_received));

		struct icmphdr *icmp_hdr = parse_icmp(m.payload);
		/* 2. Daca este un pachet IP destinat routerului, raspunde doar in
		 cazul in care acesta este un pachet ICMP ECHO request.
		Arunca pachetul original. */
		if (icmp_hdr != NULL) {
			if (ntohl(ip_hdr->daddr) == ntohl(ip_interface_received)) {
				if ((icmp_hdr->type == ICMP_ECHO) && (icmp_hdr->code == 0)) {
					send_icmp(ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost, eth_hdr->ether_shost, 0, 0, m.interface, icmp_hdr->un.echo.id, icmp_hdr->un.echo.sequence);
					continue;
				}
			}
		}

		struct arp_header *arp_hdr = parse_arp(m.payload);
		if (arp_hdr != NULL) {
			/*3. Dace este un pachet ARP Request catre un IP al routerului,
			raspunde cu ARP Reply cu adresa MAC potrivita. */
			if ((ntohs(arp_hdr->op) == ARPOP_REQUEST) && (arp_hdr->tpa == ip_interface_received)) {
				uint8_t copy_ether_shost[ETH_ALEN];
				memcpy(copy_ether_shost, eth_hdr->ether_shost, ETH_ALEN);

				get_interface_mac(m.interface, eth_hdr->ether_shost);
				memcpy(eth_hdr->ether_dhost, copy_ether_shost, sizeof(copy_ether_shost));

				send_arp(arp_hdr->spa, arp_hdr->tpa, eth_hdr, m.interface, htons(ARPOP_REPLY));
				continue;
			}
			/*4. Daca este un pachet ARP Reply, updateaza tabela ARP; daca
			exista pachete ce trebuie dirijate catre acel router, transmite-le acum.
			struct arp*/
			else if (ntohs(arp_hdr->op) == ARPOP_REPLY) {
				// fiind vorba de reply, inseamna ca s-a primit in eth_hdr->ether_shost adresa
				// pe care o cautam, deci acum putem trimite primul pachet din coada, daca acesta exista

				// idk why you would receive an arp_reply and have the queue empty
				if (queue_empty(q)) continue;
				
				// updateaza tabela arp
				arp_table[arp_table_len].ip = arp_hdr->spa;
				memcpy(arp_table[arp_table_len].mac, arp_hdr->sha, sizeof(arp_hdr->sha));
				arp_table_len++;

				// cauta daca exista pachete ce trebuie dirijate, trimite-le acum
				packet *message_waiting = queue_deq(q);
				
				struct iphdr *ip_hdr_message_queue = (struct iphdr *)(message_waiting->payload + sizeof(struct ether_header));
				struct route_table_entry *best_route = search_trie(root, ip_hdr_message_queue->daddr);
				struct ether_header *new_eth_hdr = (struct ether_header *)message_waiting->payload;

        		get_interface_mac(best_route->interface, new_eth_hdr->ether_shost);
        		memcpy(new_eth_hdr->ether_dhost, arp_hdr->sha, ETH_ALEN);
				send_packet(best_route->interface, message_waiting);
				continue;
			}
		}

		/* 5. Daca este un pachet cu TTL <= 1 trimite un mesaj ICMP corect sursei (vezi mai jos);
		arunca pachetul. */
		if (ip_hdr->ttl <= 1) {
			eth_hdr->ether_type = htons(ETHERTYPE_IP);
			send_icmp_error(ip_hdr->saddr, inet_addr(get_interface_ip(m.interface)), eth_hdr->ether_dhost, eth_hdr->ether_shost, ICMP_TIME_EXCEEDED,  0, m.interface);
			continue;
		}

		/*6. Daca este un pachet cu checksum gresit, arunca pachetul. */
		uint16_t received_checksum = ip_hdr->check;

		ip_hdr->check = 0;
		uint16_t calculated_checksum = ip_checksum(ip_hdr, sizeof(struct iphdr));


		printf("received: %hu calculated: %hu\n", received_checksum, calculated_checksum);
		if (received_checksum == calculated_checksum) {
			printf("Checksum OK\n");
		} else {
			printf("Checksum ERROR\n");
			continue;
		}

		/*7. Decrementeaza TTL, updateaza checksum.*/
		ip_hdr->ttl--;
		ip_hdr->check = ip_checksum(ip_hdr, sizeof(struct iphdr));

		/*8. Cauta intrarea cea mai specifica din tabela de rutare (numita f) astfel incat (iph􀀀 >
		daddr&f:mask == f:prefix). Odata identificata, aceasta specifica next hop pentru pachet.
		In cazul in care nu se gaseste o ruta, se trimite un mesaj ICMP sursei; arunca
		pachetul */
		struct route_table_entry *best_route = search_trie(root, ip_hdr->daddr);
		if (best_route == NULL) {
			eth_hdr->ether_type = htons(ETHERTYPE_IP);
			send_icmp_error(ip_hdr->saddr, inet_addr(get_interface_ip(m.interface)), eth_hdr->ether_dhost, eth_hdr->ether_shost, ICMP_DEST_UNREACH,  0, m.interface);
			printf("Route not found\n");
			continue;
		}

		/* 9. Modifica adresele source si destination MAC.
		Daca adresa MAC nu este cunoscuta local, genereaza un ARP request si
		transmite pe interfata destinatie. Salveaza pachetul in coada
		pentru transmitere. atunci cand adresa MAC este cunoscuta (pasul 4). */
		struct arp_entry *arp_entry = get_arp_entry(best_route->next_hop);
		if (arp_entry == NULL) {
			// save the packet for later
			packet m_copy;
			m_copy.interface = m.interface;
			m_copy.len = m.len;
			memcpy(m_copy.payload, m.payload, sizeof(m.payload));
			queue_enq(q, &m_copy);

			get_interface_mac(best_route->interface, eth_hdr->ether_shost);
			eth_hdr->ether_dhost[0] = 0xff;
			eth_hdr->ether_dhost[1] = 0xff;
			eth_hdr->ether_dhost[2] = 0xff;
			eth_hdr->ether_dhost[3] = 0xff;
			eth_hdr->ether_dhost[4] = 0xff;
			eth_hdr->ether_dhost[5] = 0xff;

			eth_hdr->ether_type = htons(ETHERTYPE_ARP);
			send_arp(best_route->next_hop, inet_addr(get_interface_ip(best_route->interface)), eth_hdr, best_route->interface, htons(ARPOP_REQUEST));
			continue;
		}
        get_interface_mac(best_route->interface, eth_hdr->ether_shost);
        memcpy(eth_hdr->ether_dhost, arp_entry->mac, sizeof(arp_entry->mac));


		/*10. Trimite pachetul mai departe folosind functia send_packet(...). */
		printf("Sending packet on interface%d\n", best_route->interface);
        send_packet(best_route->interface, &m);
	}
}
