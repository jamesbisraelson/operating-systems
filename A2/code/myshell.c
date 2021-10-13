#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"

#define ARGNUM 100
#define CMDNUM 100
#define BUFSIZE 1023


int main(int argc, char* argv[]) {
	char* buf_in;
	char buffer[BUFSIZE];
	int error = 0;

	prompt();
	buf_in = fgets(buffer, BUFSIZE, stdin);
	while(buf_in != NULL) {
		//get input
		int len = strlen(buffer);
		if(buffer[len-1] == '\n') {
			buffer[len-1] = '\0';
		}

		//tokenize input to array
		char* cmds[CMDNUM][ARGNUM];
		get_cmd_table(CMDNUM, ARGNUM, cmds, buffer);

		//check for & run internal commands
		if(!strcmp(cmds[0][0], "exit")) {
			printf("Goodbye.\n");
			return 0;
		}
		else if(!strcmp(cmds[0][0], "cd")) {
			error = chdir(cmds[0][1]);
			if(error != 0) {
				printf("Error: The directory '%s' was not found.\n", cmds[0][1]);
			}
		}

		//else run system commands
		else {
			run_cmd_pipeline(CMDNUM, ARGNUM, cmds);
		}
		prompt();
		buf_in = fgets(buffer, BUFSIZE, stdin);
	}
	return 0;
}
