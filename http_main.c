#include "main.h"
#include "http.h"

int main(int argc, char** argv) {
  int pid = fork();
  switch(pid) {
    case -1:
      return -1;
    case 0:
      usleep(2000);
      httpClient("127.0.0.1", 8080);
      printf("Closing Client\n");
      int status;
      wait(&status);
      exit(0);
    default:
      printf("starting server...\n\n");
      httpServer();
      return 0;
  }
}
