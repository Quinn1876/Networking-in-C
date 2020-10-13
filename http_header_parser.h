struct http_header {
  char content_type[100];
  int content_length;
};

typedef void (*SetHeader)(struct http_header *header, char* val);

struct parser_node {
  char value;
  struct parser_node *child;
  struct parser_node *sibling;
  SetHeader setHeader;
};

typedef void HTTP_HEADER_PARSER; /* Public Handler for _http_header_parser */

struct _http_header_parser { /* private struct */
  struct parser_node* root;
};

/**
 * @func: _htttpHeaderParser_insertHeader
 * @brief: Add a Header to the parser tree
 * @param root parser_node*: root of the tree
 * @param header char*: Header to be inserted
 * @param setter SetHeader: A function to set the header value
 */
void _httpHeaderParser_insertHeader(struct parser_node* root, char* header, SetHeader setter);

/**
 * @func: httpHeaderParser_init
 * @brief: Initialize the headerParser and save it to parser
 * @param parser HTTP_HEADER_PARSER*: pointer to the header parser handler
 */
void httpHeaderParser_init(HTTP_HEADER_PARSER** parser);

/**
 * @func: setHttpHeader
 * @brief: Parse the header and set its value accordingly
 * @param parser HTTP_HEADER_PARSER*: pointer to the header parser handler
 * @param header char*: header to set
 * @param val char*: value the header is being set to
 */
void setHttpHeader(HTTP_HEADER_PARSER* parser, struct http_header * header, char* key, char* val);

/**
 * @func: _setHttpHeaderContentType
 * @brief: Sets the content_type of the given http_header
 * @param header http_header*: Pointer to the header that is being operated on.
 * @param val void*: Will be cast to a char*
 **/
void _setHttpHeaderContentType(struct http_header *header, char *val);

/**
 * @func: _setHttpHeaderContentLength
 * @brief: Sets the content_length of the given http_header
 * @param header http_header*: Pointer to the header that is being operated on.
 * @param val void*: Will be cast to a int*
 **/
void _setHttpHeaderContentLength(struct http_header *header, char *val);

/**
 * @func: getHttpHeaderSetter
 * @brief: Gets the setter associated with header
 * @param header char*: key for the setter
 * @returns setter SetHeader
 */
SetHeader* getHttpHeaderSetter(struct parser_node *root, char *header);

/**
 * @func: freeHttpHeaderParser
 * @brief: Free the memory allocated for the Header parser
 * @param parser HTTP_HEADER_PARSER*: parser to be freed
 */
void freeHttpHeaderParser(HTTP_HEADER_PARSER* parser);

/**
 * @func: _freeParserNode
 * @brief: Dealocate the tree at root
 * @param root struct parser_node*: root of tree to be freed
 */
void _freeParserNode(struct parser_node *root);
