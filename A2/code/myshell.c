#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cmds.h"

#define BUFSIZE 1024

int main(int argc, char* argv[]) {
	char* buf_in;
	char buffer[BUFSIZE];
	int exit;

	buf_in = fgets(buffer, BUFSIZE, stdin);
	while(buf_in != NULL) {
		int len = strlen(buffer);
		if(buffer[len-1] == '\n') {
			buffer[len-1] = '\0';
		}
		printf("%s\n", buffer);
		
		exit = run_cmds(buffer);
		if(exit == 0) {
			return 0;
		}
		buf_in = fgets(buffer, BUFSIZE, stdin);
	}
	
	
	return 0;
}
