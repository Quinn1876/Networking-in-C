#include "main.h"

int startClient(int socketfd) {
  struct addrinfo hints;
  struct addrinfo *serverinfo;

  memset(&hints, 0, sizeof hints);

  hints.ai_family =AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int result = getaddrinfo(INADDR_ANY, "8000", &hints, &serverinfo);
  if (result != 0) {
    printf("failed to get server info...\n");
    return -1;
  }

  int status = connect(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen);
  freeaddrinfo(serverinfo);

  if (status != 0) {
    printf("failed to connect client with status: %d...\n", status);
    printf("Errno: %d\n", errno);
    switch(errno) {
      case EACCES:
        printf("EACCES\n");
        break;
      case EPERM:
        printf("EPERM\n");
        break;
      case EADDRINUSE:
        printf("EADDRINUSE\n");
        break;
      case EADDRNOTAVAIL:
        printf("EADDRNOTAVAIL\n");
        break;
      case EAFNOSUPPORT:
        printf("EAFNOSUPPORT\n");
        break;
      case EAGAIN:
        printf("EAGAIN \n");
        break;
      case EALREADY:
        printf("EALREADY\n");
        break;
      case EBADF:
        printf("EBADF\n");
        break;
      case ECONNREFUSED:
        printf("ECONNREFUSED\n");
        break;
      case EFAULT:
        printf("EFAULT\n");
        break;
      case EINPROGRESS:
        printf("EINPROGRESS\n");
        break;
      case EINTR:
        printf("EINTR\n");
        break;
      case EISCONN:
        printf("EISCONN\n");
        break;
      case ENETUNREACH:
        printf("ENETUNREACH\n");
        break;
      case ENOTSOCK:
        printf("ENOTSOCK\n");
        break;
      case EPROTOTYPE:
        printf("EPROTOTYPE\n");
        break;
      case ETIMEDOUT:
        printf("ETIMEDOUT\n");
        break;
    }
    return -1;
  }

  AMessage msg = AMESSAGE__INIT; // AMessage
  void *buf;                     // Buffer to store serialized data
  int len;                  // Length of serialized data

  msg.a = 15;
  msg.has_b = 1;
  msg.b = 10;
  len = amessage__get_packed_size(&msg);

  buf = malloc(len);
  amessage__pack(&msg,buf);

  fprintf(stderr,"Sending %d serialized bytes\n",len); // See the length of message
  sendSocketMessage(socketfd, buf, &len);

  usleep(5000);
  U8 buffer[MAX_MSG_SIZE];
  int bufSize = MAX_MSG_SIZE;
  printf("Waiting for Response from server\n");
  readSocketMessage(socketfd, buffer, (U32)bufSize);

  buffer[bufSize-1] = '\0';

  printf("buffer: %s\n", buffer);

  free(buf); // Free the allocated serialized buffer
  return 0;
}
