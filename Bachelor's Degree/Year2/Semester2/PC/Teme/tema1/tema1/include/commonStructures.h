#include "stdint.h"
#include <asm/types.h>

struct route_table_entry {
	uint32_t prefix;
	uint32_t next_hop;
	uint32_t mask;
	int interface;
} __attribute__((packed));

struct arp_entry {
	__u32 ip;
	uint8_t mac[6];
};