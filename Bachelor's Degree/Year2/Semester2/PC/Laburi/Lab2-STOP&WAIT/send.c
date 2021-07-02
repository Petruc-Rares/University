#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"

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

void sendFile(msg t, char *fileName) {
  // trimite numele fisierului

  sprintf(t.payload,"%s", fileName);
  t.len = strlen(t.payload)+1;
  send_message(&t);
  
  // asteapta acknowledge
  checkAcknowledge(t);

  // deschide fisierul
  int sourceFile = open(fileName, O_RDONLY);
  int sizeFile = lseek(sourceFile, 0, SEEK_END);

  sprintf(t.payload,"%d", sizeFile);
  t.len = strlen(t.payload)+1;
  send_message(&t);

  // asteapta acknowledge
  checkAcknowledge(t);

  int chunk;

  // restauram pointerul la inceputul fisierului
  lseek(sourceFile, 0, SEEK_SET);

  // dupa urmeaza while-ul
  // read data
  while((chunk = read(sourceFile, t.payload, MAX_LEN)) != 0) {
    t.len = chunk;

    // send data
    send_message(&t);

    // receive ack
    checkAcknowledge(t);
  }

  return;
}

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;

  if (argc != 2) {
    printf("Run ./run_experiment.sh file_name");
    exit(1);
  }

  sendFile(t, argv[1]);

  return 0;
}
