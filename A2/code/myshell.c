#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cmds.h"

#define ARGNUM 100
#define BUFSIZE 2048
#define DELIM " "

int main(int argc, char* argv[]) {
	char* buf_in;
	char buffer[BUFSIZE];
	int error = 0;

	prompt();
	buf_in = fgets(buffer, BUFSIZE, stdin);
	while(buf_in != NULL) {
		int len = strlen(buffer);
		if(buffer[len-1] == '\n') {
			buffer[len-1] = '\0';
		}

		char* arg_arr[ARGNUM];
		tokenize_to_array(arg_arr, ARGNUM, buffer, DELIM);
	
		int i;
		for(i=0; i<ARGNUM; i++) {
			if(arg_arr[i] == NULL) break;
			printf("%s\n", arg_arr[i]);
		}
		
		//check for & run internal commands
		if(!strcmp(arg_arr[0], "exit")) {
			printf("Goodbye.\n");
			return 0;
		}
		else if(!strcmp(arg_arr[0], "cd")) {
			error = chdir(arg_arr[1]);
			if(error != 0) {
				printf("Error: The directory was not found.\n");
			}
		}
		//else run system commands
		else {
			error = run_cmds(arg_arr, ARGNUM);
			if(error != 0) {
				printf("Error: The system command could not be run.\n");
			}
		}	
		prompt();
		buf_in = fgets(buffer, BUFSIZE, stdin);
	}
	return 0;
}
