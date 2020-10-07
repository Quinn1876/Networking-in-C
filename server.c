#include "main.h"

int startServer(int socketfd) {
  printf("Starting Server...\n");

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons( 8000 );
  addr.sin_addr.s_addr = htonl( INADDR_ANY );

  int res = bind( socketfd, (struct sockaddr*) &addr, sizeof( addr ));
  if ( res == -1 ) {
    printf("Error Binding: %d\n", errno);
    return res;
  }

  res = listen(socketfd, 20); /* Actively Listening for Communications on Socket */
  if ( res == -1 ) {
    printf("Error Listening: %d\n", errno);
    return res;
  }
  printf("Listening on 8000...\n");

  int activeSocket;
  struct sockaddr client_addr;
  socklen_t client_addr_length;

  activeSocket = accept(socketfd, &client_addr, &client_addr_length);

  AMessage *msg;

  U8 buf[MAX_MSG_SIZE];
  int bufSize = MAX_MSG_SIZE;
  size_t msg_len = readSocketMessage(activeSocket, buf, MAX_MSG_SIZE);

  buf[msg_len] = '\0';
  printf("%s\n", buf);

  /* Unpack the message */
  msg = amessage__unpack(NULL, msg_len, buf);
  if (msg == NULL) {
    printf("Error unpacking incoming message\n");
    exit(-1);
  }

  printf("Received: a=%d",msg->a);
  if (msg->has_b)
    printf("  b=%d",msg->b);
  printf("\n");


  U8* response = (U8*)"Helllo from the server";
  int responseLength = 24;

  sendSocketMessage(activeSocket, response, &responseLength);
  printf("Message Sent: %s\n", response);

  close(activeSocket);

  return 0;
}
