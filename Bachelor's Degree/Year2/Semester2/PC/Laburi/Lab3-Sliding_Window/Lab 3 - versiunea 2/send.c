#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

void check_response(msg* t) {
  if (recv_message(t)<0){
    perror("Receive error ...");
    exit(1);
  }
  else {
    printf("[send] Got reply with payload: %s\n", t->payload);
  }
}

void send_file(char* file_name, int BDP) {
  int windowSize = (BDP * 1000) / (sizeof(msg) * 8);

  // Open file
  int fd = open(file_name, O_RDONLY);
  if (fd < 0) {
    perror("Can't open file");
    exit(1);
  }

  msg m;

  // Send filename
  sprintf(m.payload, "%s_copy", file_name);
  m.len = strlen(m.payload) + 1;
  m.checksum = getChecksum(m.payload, m.len);
  printf("[send] Sending file_name\n");
  send_message(&m);
  check_response(&m);

  // Send offset
  int offset = lseek(fd, 0, SEEK_END);
  sprintf(m.payload, "%d", offset);
  m.len = strlen(m.payload) + 1;
  m.checksum = getChecksum(m.payload, m.len);
  printf("[send] Sending offset\n");
  send_message(&m);
  check_response(&m);

  // Send packets
  lseek(fd, 0, SEEK_SET);

  int packet_size;
  int packet_no = 1;

  // Start sliding window
  for (int i = 0; i < windowSize; i++) {
	  packet_size = read(fd, m.payload, MSGSIZE);
	  if (packet_size == 0) break;

	  m.len = packet_size;
	  m.checksum = getChecksum(m.payload, m.len);

	  send_message(&m);
    packet_no++;
  }

  // If file is bigger than sliding window
  while ((packet_size = read(fd, m.payload, MSGSIZE))) {
    if (packet_size < 0) {
      perror("Error reading file");
      exit(1);
    }

    m.len = packet_size;
    m.checksum = getChecksum(m.payload, m.len);

    printf("[send] Sending packet %d\n", packet_no);

    send_message(&m);
    check_response(&m);
    packet_no++;
  }

  for (int i = 0; i < windowSize; i++) {
	  check_response(&m);
  }

  close(fd);
}


int main(int argc,char** argv){
  init(HOST,PORT);
  if (argc != 3) {
    printf("Usage: ./run_experiment bdp filename\n");
    exit(1);
  }
  send_file(argv[2], atoi(argv[1]));
  return 0;
}
