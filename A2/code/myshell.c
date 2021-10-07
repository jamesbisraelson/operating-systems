#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"

#define ARGNUM 100
#define CMDNUM 100
#define BUFSIZE 2048
#define SPACEDELIM " "
#define PIPEDELIM "|"

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
		char* tokens[CMDNUM];

		tokenize_to_array(CMDNUM, tokens, buffer, PIPEDELIM);

		//check for & run internal commands
		if(!strcmp(tokens[0], "exit")) {
			printf("Goodbye.\n");
			return 0;
		}
		else if(!strncmp(tokens[0], "cd", 2)) {// the length of the "cd" command is 2
			char* cd[2];//a cd command should only have two arguments, "cd" and the directory
			tokenize_to_array(2, cd, tokens[0], SPACEDELIM);
			error = chdir(cd[1]);//i don't think these count as magic numbers. sue me.
			if(error != 0) {
				printf("Error: The directory '%s' was not found.\n", cd[1]);
			}
		}

		//else run system commands
		else {
			run_cmds(CMDNUM, tokens, ARGNUM, SPACEDELIM);
		}
		prompt();
		buf_in = fgets(buffer, BUFSIZE, stdin);
	}
	return 0;
}
