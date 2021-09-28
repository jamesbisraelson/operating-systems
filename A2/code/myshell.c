#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cmds.h"

#define BUFSIZE 2048

int main(int argc, char* argv[]) {
	char* buf_in;
	char buffer[BUFSIZE];

	prompt();
	buf_in = fgets(buffer, BUFSIZE, stdin);
	while(buf_in != NULL) {
		int len = strlen(buffer);
		if(buffer[len-1] == '\n') {
			buffer[len-1] = '\0';
		}
		if(!strcmp(buffer, "exit")) {
			return 0;
		}
		
		run_cmds(buffer);
		prompt();
		buf_in = fgets(buffer, BUFSIZE, stdin);
	}
	return 0;
}
