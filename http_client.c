#include "main.h"
#include "http.h"

int httpClient(char* address, short port) {
  int client_socket;
  if(getHttpSocket(&client_socket) != 0) {
    printf("Error geting CLient socket\n\n");
    return -1;
  }

  struct sockaddr_in remote_address;
  remote_address.sin_family = AF_INET;
  remote_address.sin_port = htons(port);
  inet_aton(address, &remote_address.sin_addr.s_addr);

  if(connect(client_socket, (struct sockaddr*) &remote_address, sizeof(remote_address)) != 0) {
    printf("Error connecting to remote\n\n");
    return -1;
  }

  char request[] = "GET / HTTP/1.1\r\n\r\n";
  char response[4096];

  send(client_socket, request, sizeof(request), 0);
  recv(client_socket, response, sizeof(response), 0);

  printf("response from server %s\n", response);

  close(client_socket);

  return 0;
}
