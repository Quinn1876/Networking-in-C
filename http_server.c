#include "main.h"
#include "http.h"
#include "http_header_parser.h"

#define REQUEST_BUFFER_SIZE 4096
// #define DEBUG_HTTP_SERVER_LOGGING 0

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
  sprintf(http_header, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", header_size);
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
  char request_buffer[REQUEST_BUFFER_SIZE];
  char method[10];
  char request[2048];
  char version[10];

  HTTP_HEADER_PARSER* parser;
  httpHeaderParser_init(&parser);


  do { /* *** Main Loop for the Server *** */

    /* Accept client connection - Don't store the client info */
    client_socket = accept(server_socket, NULL, NULL);
    printf("Client Conected\n");

    /* Get the info from the request and read it into memory */
    recv(client_socket, request_buffer, REQUEST_BUFFER_SIZE, 0);

    #ifdef DEBUG_HTTP_SERVER_LOGGING
      printf("Request Buffer: \n%s\n", request_buffer);
    #endif /* DEBUG_HTTP_SERVER_LOGGING */

    /* Declare Variables */
    int line_start = 0;
    int request_buffer_cursor = 0;
    char header_key[100];
    char header_value[100];
    struct http_header header;
    memset(header.content_type, 0, sizeof(header.content_type)); /* TODO: Write an INIT func for http header */
    header.content_length = 0;


    /**
     * Http Headers are ended by \r\n\r\n
     * So if after the first newline is found, the second will be found imediatly with a distance of 2
     */
    while (request_buffer_cursor - line_start != 2) {/* WHILE IN HEADER */
      if (request_buffer_cursor == 0) {
        /* Read the first line of the request header */
        sscanf(request_buffer, "%s %s HTTP/%s\r\n", method, request, version); /* TODO: Error Handling */
      } else if ((sscanf(request_buffer + request_buffer_cursor, "%s %s\r\n", header_key, header_value)) > 0) { /* Read the header and its value */
          httpHeaderParser_setHttpHeader(parser, &header, header_key, header_value);
      }

      /* Parse to the end of the line */
      for (
        line_start = request_buffer_cursor;
        request_buffer[request_buffer_cursor] != '\r' && request_buffer[request_buffer_cursor] != '\n' && request_buffer_cursor < REQUEST_BUFFER_SIZE;
         ++request_buffer_cursor
      ) {}
      /* Parse past the EOL characters */
      if (request_buffer[request_buffer_cursor] == '\r')
        ++request_buffer_cursor;
      if (request_buffer[request_buffer_cursor] == '\n')
        ++request_buffer_cursor;
    }

    /* Log info */
    printf("Client Message METHOD: %s\n\n", method);
    printf("Client Message VERSION: %s\n\n", version);
    printf("Content - Type: %s\n", header.content_type);
    printf("Content - length: %d\n", header.content_length);


    /* Send a Response */
    send(client_socket, http_header, header_size, 0);
    close(client_socket);
  } while (1);

  close(server_socket);
  httpHeaderParser_destroy(parser);

  return 0;
}

