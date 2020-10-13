/**
 * @func: httpServer
 * @brief: A tutorial Http Server Example from Eduonix
 * @returns:  0 := success
 *          <>0 ;+ failure
 */
int httpServer();

/**
 * @func: httpClient
 * @brief: A tutorial Http Client Example from Eduonix
 * @param address char*: address that you are connecting to
 * @returns:  0 := success
 *          <>0 ;+ failure
 */
int httpClient(char* address, short port);
