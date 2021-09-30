#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"

#define CWDBUF 256
#define PIPE "|"

int run_cmds(char** arg_arr, const int arg_size) {
	int pid = fork();
	int return_code;
	if(pid != 0) {
		wait(NULL);
	}
	else {
		char* cmd = arg_arr[0];
		//char* arguments = malloc(arg_size * sizeof(char*));
		return_code = execvp(cmd, arg_arr);
		if(return_code == 0) {
			printf("Error: Could not execute %s", cmd);
			return -1;
		}
			/*if(!strcmp(arg_arr[i], PIPE)) {
				return_code = execvp(cmd, arguments);
				if(return_code == 0) {
					printf("Error: Could not execute %s", cmd);
					return -1;
				}
				j = 0;
				free(arguments);
				arguments = malloc(arg_size * sizeof(char*));
			}*/
	}
	return 0;
}

void tokenize_to_array(char** arg_arr, const int arr_size, char* buffer, const char* delim) {	
	char* token;
	char* save_ptr;
	int i;

	for(i=0; i<arr_size; i++) {
		token = strtok_r(buffer, delim, &save_ptr);
		buffer = NULL;
		arg_arr[i] = token;
	}
}

void prompt() {
	char cwd[256];
	if(getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("%s", cwd);
		printf(" %% ");
	}
}
