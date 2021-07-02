#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

void send_ack() {
  msg r;
  sprintf(r.payload,"%s", "ACK");
  r.len = strlen(r.payload) + 1;
  send_message(&r);
  printf("[recv] ACK sent\n");
}

void check_transmission(msg* r) {
  if (recv_message(r)<0){
    perror("Failed to receive message");
    exit(1);
  }
}

void receive_file() {
  msg r;

  // Receive filename
  check_transmission(&r);
  printf("[recv] Got filename\n");
  char* file_name = r.payload;
  send_ack();

  // Create file
  int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (fd < 0) {
    perror("Can't create file");
    exit(1);
  }

  // Receive offset
  check_transmission(&r);
  printf("[recv] Got offset\n");
  int offset;
  sscanf(r.payload, "%d", &offset);
  send_ack();

  // Receive packets
  int received_size = 0;
  int packet_no = 1;
  while (received_size < offset) {
    check_transmission(&r);

    if (r.checksum == getChecksum(r.payload, r.len)) {
      send_ack();
      printf("[recv] Got packet %d, checksum OK\n", packet_no);
    } else {
      printf("Checksum fail");
    }

    int bytes_written = write(fd, r.payload, r.len);
    if (bytes_written < 0) {
      perror("Failed writing to file");
      exit(1);
    }

    received_size += r.len;
    printf("Transfer progress: %d/%d\n", received_size, offset);
    packet_no++;
  }

  close(fd);
}


int main(int argc,char** argv){
  init(HOST,PORT);
  receive_file();
  return 0;
}
