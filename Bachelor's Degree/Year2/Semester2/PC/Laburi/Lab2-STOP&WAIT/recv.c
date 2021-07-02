#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

void sendAck(msg r) {
  sprintf(r.payload,"%s", "ACK");
  r.len = strlen(r.payload) + 1;
  send_message(&r);
  printf("[recv] ACK sent\n");

  return ;
}

void receiveFile(msg r) {

  // get file name

  if (recv_message(&r)<0) {
    perror("Receive message");
    exit(-1);
  }
  
  // Send ACK for name of file
  sendAck(r);

  // create the copy of the file received
  int destinationFile = open(strcat(r.payload, "_copy"), O_WRONLY | O_CREAT | O_TRUNC, 0777);

  // get file size
  if (recv_message(&r)<0) {
    perror("Receive message");
    exit(-1);
  }

  // convert to int
  int fileSize = atoi(r.payload);

  // Send ACK for file size
  sendAck(r);

  int receivedSize = 0;

  while (receivedSize < fileSize) {

    // receive
    if (recv_message(&r)<0) {
      perror("Receive message");
      exit(-1);
    }

    // send ack
    sendAck(r);

    // write the content to the destination file
    write(destinationFile, r.payload, r.len);    

    receivedSize += r.len;
  }

  close(destinationFile);

  return ;
}

int main(int argc,char** argv){
  msg r;
  init(HOST,PORT);

  receiveFile(r);

  return 0;
}
