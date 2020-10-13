#include "main.h"
#include "http.h"

int httpServer() {
  /* Open the html file that the server will send back in the default response */
  FILE* html_data;
  html_data = fopen("./static/index.html", "r");
  int html_fileSize = getFileSize("./static/index.html");
  if (html_fileSize < 0) {
    printf("Error getting file size\n");
    return -1;
  }

  char response_data[html_fileSize];
  memset(response_data, 0, html_fileSize);
  if ((fread(response_data, html_fileSize, 1, html_data)) < 0) {
    return -1;
  }

  int header_size = (1024 + html_fileSize);
  char *http_header = (char *)malloc(header_size);
  memset(http_header, 0, header_size);
  strcpy(http_header, "HTTP/1.1 200 OK\r\n\r\n");
  strcat(http_header, response_data);


  /* Get the Server Socket */
  int server_socket;
  int response = getHttpSocket(&server_socket);

  if (response != 0) {
    printf("Error getting Socket.\n\n");
    return -1;
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int bind_res = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
  if (bind_res != 0) {
    printf("Error Binding: %s\n\n", strerror(errno));
    return -1;
  }

  int listen_res = listen(server_socket, 5);
  if (listen_res != 0) {
    printf("Error Listening\n\n");
    return -1;
  }

  printf("Now accepting communication on 0.0.0.0:8080\n\n");

  int client_socket;
  char buf[4096];
  char method[10];
  char request[2048];
  char version[10];
  U32 bufcursor;
  while (1) {
    /* Don't store the client info */
    client_socket = accept(server_socket, NULL, NULL);
    printf("Client Conected\n");

    /* Get the info from the request and read it into memory */
    recv(client_socket, buf, 4096, 0);
    int line_start = 0;
    int bufcursor = 0;
    char temp_header[100];
    char temp_header_value[100];
    while (bufcursor - line_start != 2) {/* WHILE IN HEADER */
      /* Read the header and its value */
      scanf(buf + bufcursor, "%s:%s\r\n", temp_header, temp_header_value);

      /* Parse to the end of the line */
      for(line_start = bufcursor; buf[bufcursor] != '\r' || buf[bufcursor] != '\n'; ++bufcursor) {}
      /* Parse past the EOL characters */
      if (buf[bufcursor] == '\r')
        ++bufcursor;
      if (buf[bufcursor] == '\n')
        ++bufcursor;
    }


    sscanf(buf, "%s %s HTTP/%s\r\n\r\n", method, request, version);



    printf("Client Message:\n%s\n", buf);
    printf("Client Message METHOD: %s\n\n", method);
    printf("Client Message VERSION: %s\n\n", version);


    /* Send a Response */
    send(client_socket, http_header, header_size, 0);
    close(client_socket);
  }
}

