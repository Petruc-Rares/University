#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

void checkAcknowledge(msg t) {
  // Check response:
  if (recv_message(&t)<0){
    perror("Receive error ...");
    exit(-1);
  }
  else {
    printf("[send] Got reply with payload: %s\n", t.payload);
  }
}

void sendFile(msg t, int BDP, char *fileName) {
  int windowSize = (BDP * 1000) / (sizeof(msg) * 8);

  // trimite numele fisierului

  sprintf(t.payload,"%s", fileName);
  t.len = strlen(t.payload)+1;
  t.checksum = getChecksum(t.payload, t.len);
  send_message(&t);

  // asteapta acknowledge
  checkAcknowledge(t);

  // deschide fisierul
  int sourceFile = open(fileName, O_RDONLY);
  int sizeFile = lseek(sourceFile, 0, SEEK_END);

  sprintf(t.payload,"%d", sizeFile);
  t.len = strlen(t.payload)+1;
  t.checksum = getChecksum(t.payload, t.len);
  send_message(&t);

  // asteapta acknowledge
  checkAcknowledge(t);

  int chunk;

  // restauram pointerul la inceputul fisierului
  lseek(sourceFile, 0, SEEK_SET);

  // pornim fereastra glisanta
  for (int i = 0; i < windowSize; i++) {
	  chunk = read(sourceFile, t.payload, MSGSIZE);
	  if (chunk == 0) break;

	  t.len = chunk;
	  t.checksum = getChecksum(t.payload, t.len);

	  send_message(&t);
  }

  // dupa urmeaza while-ul, daca depasim fereastra
  // read data
  while((chunk = read(sourceFile, t.payload, MSGSIZE)) != 0) {
    // receive ack
	  checkAcknowledge(t);

    t.len = chunk;
    t.checksum = getChecksum(t.payload, t.len);

    // send data
    send_message(&t);
  }

  for (int i = 0; i < windowSize; i++) {
	  checkAcknowledge(t);
  }
}

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;
  if (argc != 3) {
    printf("Run ./run_experiment.sh bandwith_size file_name");
    exit(1);
  }

  sendFile(t, atoi(argv[1]), argv[2]);

  return 0;
}

