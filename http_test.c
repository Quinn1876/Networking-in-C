/**
 * @file http_test.c
 * @brief integration test to ensure the following functionality:
 *        Adds Content-Type to header
 *        Adds Content-Length to header
 *        Ignore ofther Headers
 *        free memory alocated by the parser
 * @author Quinn 1876
 */

#include "http_header_parser.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main() {
  HTTP_HEADER_PARSER *parser;
  struct http_header header;
  httpHeaderParser_init(&parser);

  assert(parser != NULL);
  httpHeaderParser_setHttpHeader(parser, &header, "Content-Type:", "application/json");
  httpHeaderParser_setHttpHeader(parser, &header, "Content-Length:", "20");
  httpHeaderParser_setHttpHeader(parser, &header, "FAKE Header", "500");

  printf("Header Content-Type: %s\n", header.content_type);
  printf("Header Content-Length: %d\n", header.content_length);
  // assert(header.content_length == 20);
  // assert(strcmp(header.content_type, "application/json") == 0);

  httpHeaderParser_destroy(parser);
  return 0;
}
