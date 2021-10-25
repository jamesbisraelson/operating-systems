#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFSIZE 81

int main(int argc, char*argv[]) {

  char* cmd;
  char buffer[BUFSIZE];
  int return_code;
  int pid;
  int len;

  printf("> ");
  cmd = fgets(buffer, BUFSIZE, stdin);

  while(cmd!=NULL) {

    len = strlen(buffer);
    if(buffer[len-1] == '\n') {
      buffer[len-1] = '\0';
    }

    pid=fork();

    if(pid != 0){
      wait(NULL); 
    }
    else {
      return_code = execlp(cmd, cmd, NULL);
      if(return_code != 0) {
        printf("Error executing %s\n", cmd);
        exit(0);
      }
    }
    printf("> ");
    cmd = fgets(buffer, BUFSIZE, stdin);
  }
  return 0;
}
