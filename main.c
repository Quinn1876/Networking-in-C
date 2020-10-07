/**
 * @file: main.c
 *
 * @brief: Entrypoint to the application
 *
 * @author: Quinn1876
 *
 */
#include "main.h"

#define RUN_CHILD_CLIENT 1

int main(int argc, char** argv) {
  int socketfd;
  if (getHttpSocket(&socketfd) != 0) {
    printf("Error gettings Server Socket...\n");
    return -1;
  }

  if (RUN_CHILD_CLIENT) {
    int pid = fork();
    if (pid == 0) {
      int clientSockfd;
      if (getHttpSocket(&clientSockfd) != 0) {
        printf("Error gettings Client Socket...\n");
        return -1;
      }
      usleep(4000); /* Sleep 4 seconds to ensure that the server is setup and ready */
      startClient(clientSockfd);
      close(clientSockfd);
      exit(0);
    }
  }

  startServer(socketfd);
  close(socketfd);

  int status;
  wait(&status);
  return 0;
}
