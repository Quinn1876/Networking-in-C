#pragma once
/**
 * @file: main.h
 *
 * @brief:  Currently contains all of the include for all third party and standard library
 *          Includes. Also contains all function declarations and descriptions of those
 *          functions.
 *
 * @author: Quinn 1876
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> /* hton and ntoh functions */

#include "./proto/amessage.pb-c.h"
#define MAX_MSG_SIZE 1024

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;


int main(int argc, char** argv);

/**
 * @func: getHttpSocket
 * @brief: Creates a socket and saves it's file descriptor to socket
 * @param socket: pointer that will be set to the value of the socketfd on successt
 * @returns: 0 := success
 *         <>0 := failure
 */
int getHttpSocket(int* socketfd);

/**
 * @func: readSocketMessage
 * @brief: Read a message from socket into buffer
 * @param socketfd int: The file that the data is coming from
 * @param buffer U8*: arbitrary buffer for the data
 * @param length U32: the maximum size of the buffer
 * @returns: total number of bytes read
 * @attribution: Protobuf-c examples: https://github.com/protobuf-c/protobuf-c/wiki/Examples
 */
size_t readSocketMessage(int socketfd, U8* buffer, U32 length);

/**
 * @func: sendSocketMessage
 * @brief: Send a message from buff to a server over socket
 * @param socketfd int: Socket
 * @param buf U8*: Data to be sent
 * @param len int*: number of bytes to be sent from the buffer
 * @attribution: ECE 252
 */
int sendSocketMessage(int socketfd, U8 *buf, int *len);

/**
 * @func: getFileSize
 * @brief: return the size of a file, in bytes
 * @param filePath const char*: the path of the file which you wish to know the size of.
 * @return  int fileSize in bytes;
 *          < 0 := failure
 */
int getFileSize(const char* filePath);

/**
 * @func: startServer
 * @brief: Opens the Server on the given socket
 * @param socketfd int: the file descriptor of the socket to bind the server to.
 * @returns: 0 := success
 *         <>0 := failure
*/
int startServer(int socketfd);

/**
 * @func: startClient
 * @brief: Opens a Client which sends a test message to the server
 * @param socketfd int: Socket for the client to connect to
 * @returns:  0 := success
 *          <>0 := failure
 */
int startClient(int socketfd);

