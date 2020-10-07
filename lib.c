#include "main.h"

int getHttpSocket(int* socketfd ) {
  int res;
  res = socket(AF_INET, SOCK_STREAM, 0);

  if (res == -1) {
    printf("Error Creating Socket: %d\n", errno);
    return -1;
  }

  *socketfd = res;
  return 0;
}

size_t readSocketMessage(int socketfd, U8* buffer, U32 length) {
  size_t cur_len = 0;
  size_t nread;
  while ((nread=recv(socketfd, buffer + cur_len, length - cur_len, 0)) != 0) {
    cur_len += nread;
    if (cur_len == length)
    {
      fprintf(stderr, "max message length exceeded\n");
      exit(1);
    }
  }
  return cur_len;
}

int sendSocketMessage(int socketfd, U8 *buf, int *len) {
  int total = 0;
  int bytesleft = *len;
  int n;

  while (total < *len) {
    n = send(socketfd, buf + total, bytesleft, 0);
    if (n == -1) {
      break;
    }
    total += n;
    bytesleft -=n;
  }
  *len = total;
  return n == -1 ? -1 : 0;
}
