#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"

#define CWDBUF 256

int get_piped_arr(char** arg_arr, const int arg_arr_size) {
	int i, num_pipes;
	for(i=0, num_pipes=0; i<arg_arr_size; i++) {
		if(!strcmp(arg_arr[i], "|")) {
			arg_arr[i] = NULL;
			num_pipes++;
		}
	}
	return num_pipes;
}

int run_cmds(const int cmds_num, char* cmds_arr[], const int args_num, char* delim) {
	return run_cmds_recursive(0, STDIN_FILENO, cmds_num, cmds_arr, args_num, delim);
}

int run_cmds_recursive(int index, int pipein, const int cmds_num, char* cmds_arr[], const int args_num, char* delim) {
	char* args_arr[args_num];
	tokenize_to_array(args_num, args_arr, cmds_arr[index], delim);
	
	int pid = fork();

	if(pid == 0) {
		if(index < cmds_num) {
			//redirect pipein read to stdin
			dup2(pipein, STDIN_FILENO);
			
			//create pipeout
			int pipeout[2];
			pipe(pipeout);
			close(pipeout[0]);
	
			//redirect stdout to pipeout
			dup2(STDOUT_FILENO, pipeout[1]);
			close(pipeout[1]);
	
			//execvp
			execvp(args_arr[0], args_arr);
			
			//call the function recursively for the next command to be piped
			index++;
			return run_cmds_recursive(index, pipeout[1], cmds_num, cmds_arr, args_num, delim);
		}
		if(index == cmds_num) {	
			//redirect pipein read to stdin
			dup2(pipein, STDIN_FILENO);
			close(pipein);
	
			//execvp
			execvp(args_arr[0], args_arr);
	
			//all done
			return 0;
		}
	}
	else {
		close(pipein);
		wait(NULL);
		if(pid == -1) {
			perror("fork error");
			exit(1);
		}
	}
	return 1;
}

void tokenize_to_array(const int token_num, char* token_arr[token_num], char* str_in, char* delim) {
	char* token;
	char* save_ptr;
	int i=0;

	token = strtok_r(str_in, delim, &save_ptr);
	str_in = NULL;

	while(i<token_num) {
		token_arr[i] = token;
		token = strtok_r(str_in, delim, &save_ptr);
		str_in = NULL;
		i++;
	}
}

void printtokens(const int token_num, char* token_arr[]) {
	int i;
	for(i=0; i<token_num; i++) {
		if(token_arr[i]==NULL) break;
		printf("%s\n", token_arr[i]);
	}
}

void prompt() {
	char cwd[CWDBUF];
	if(getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("%s", cwd);
		printf(" %% ");
	}
}
