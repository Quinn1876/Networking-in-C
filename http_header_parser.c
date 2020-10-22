#include "main.h"
#include "http_header_parser.h"

void _httpHeaderParser_insertHeader(
  struct parser_node* root,
  char* header,
  SetHeader setter
) {
  if (root->value == *header) {
    if (*header == '\0') {
      root->setHeader = setter;
      return;
    }
    if (root->child == NULL) {
      root->child = malloc(sizeof( struct parser_node ));
      root->child->sibling = NULL;
      root->child->child = NULL;
      root->child->setHeader = NULL;
      root->child->value = *(header+1);
    }
    return _httpHeaderParser_insertHeader(root->child, header + 1, setter);
  } else {
    if (root->sibling == NULL) {
      root->sibling = malloc(sizeof( struct parser_node ));
      root->sibling->sibling = NULL;
      root->sibling->child = NULL;
      root->sibling->value = *header;
      root->sibling->setHeader = NULL;
    }
    return _httpHeaderParser_insertHeader(root->sibling, header, setter);
  }
}

void httpHeaderParser_init(HTTP_HEADER_PARSER** parser) {
  *parser = malloc(sizeof(struct _http_header_parser));
  struct _http_header_parser* p = (struct  _http_header_parser *) *parser;
  p->root = malloc(sizeof(struct parser_node));
  p->root->child = NULL;
  p->root->sibling = NULL;
  p->root->setHeader = NULL;
  p->root->value = '\0';
  _httpHeaderParser_insertHeader(p->root, "Content-Type:", &_setHttpHeaderContentType);
  _httpHeaderParser_insertHeader(p->root, "Content-Length:", &_setHttpHeaderContentLength);
  assert(parser != NULL);
  assert(p != NULL);
}

void httpHeaderParser_setHttpHeader(HTTP_HEADER_PARSER* parser, struct http_header * header, char* key, char* val) {
  assert(parser != NULL);
  struct _http_header_parser* p = (struct  _http_header_parser *) parser;
  assert(p != NULL);
  SetHeader* setter = getHttpHeaderSetter(p->root, key);
  if (setter != NULL) {
    (*setter)(header, val);
  }
}

void _setHttpHeaderContentType(struct http_header *header, char *val) {
  strcpy(header->content_type,  val);
}

void _setHttpHeaderContentLength(struct http_header *header, char *val) {
  header->content_length = atoi(val);
}

SetHeader* getHttpHeaderSetter(struct parser_node *root, char *header) {
  if (root->value == *header) {
    if (*header == '\0')
      return &(root->setHeader);
    else if (root->child == NULL)
      return NULL;
    else
      return getHttpHeaderSetter(root->child, header+1);
  } else {
    if (root->sibling == NULL)
      return NULL;
    return getHttpHeaderSetter(root->sibling, header);
  }
}

void httpHeaderParser_destroy(HTTP_HEADER_PARSER* parser) {
  struct _http_header_parser* p = (struct  _http_header_parser *) parser;
  _freeParserNode(p->root);
  free(p);
  p = NULL;
}

void _freeParserNode(struct parser_node *root) {
  if (root->sibling != NULL)
    _freeParserNode(root->sibling);
  if (root->child != NULL)
    _freeParserNode(root->child);

  free(root);
}
