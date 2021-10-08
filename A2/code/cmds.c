#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "cmds.h"

#define CWDBUF 256
#define PIPEDELIM "|"
#define SPACEDELIM " "

/*
 * function: tokenize_to_array
 * ---------------------------
 * splits a string into tokens and stores each token in an array.
 *
 * token_num: length of token_arr
 * token_arr: the array in which to store the tokens
 * str_in: the string to be split
 * delim: the delimiter used to split the string
 */
void tokenize_to_array(const int token_num, char* token_arr[token_num], char* str_in, char* delim) {
	char* token;
	char* save_ptr;
	int i;
	
	for(i=0; i<token_num; i++) {
		token = strtok_r(str_in, delim, &save_ptr);
		str_in = NULL;

		token_arr[i] = token;
	}
}


void get_cmd_table(const int cmds_num, const int args_num, char* cmds_arr[cmds_num][args_num], char* str_in) {
	char* token_arr[cmds_num];
	tokenize_to_array(cmds_num, token_arr, str_in, PIPEDELIM);
	
	int i = 0;
	while(i<cmds_num) {
		tokenize_to_array(args_num, cmds_arr[i], token_arr[i], SPACEDELIM);
		i++;
	}
}

void run_cmd_pipeline(const int cmds_num, const int args_num, char* cmds_arr[cmds_num][args_num]) {
	int pipefd[2];
	pid_t pid;
	int fd_in = STDIN_FILENO;
	
	int i = 0;
	while(cmds_arr[i][0] != NULL) {
		pipe(pipefd);

		if((pid=fork()) == -1) {
			perror("fork error");
			exit(1);
		}
		if(pid == 0) {//child
			dup2(fd_in, STDIN_FILENO);
			if(cmds_arr[i+1][0] != NULL) {
				dup2(pipefd[1], STDOUT_FILENO);
			}
			close(fd_in);
			execvp(cmds_arr[i][0], cmds_arr[i]);
			exit(1);
		}
		else {//parent
			wait(NULL);
			close(pipefd[1]);
			fd_in = pipefd[0];
			i++;
		}
	}
}

void print_cmd_table(const int i_max, const int j_max, char* table[i_max][j_max]) {
	int i = 0;
	while(i<i_max && table[i] != NULL) {
		print_tokens(j_max, table[i]);
		i++;
	}
}

void print_tokens(const int token_num, char* token_arr[token_num]) {
	int i = 0;
	while(i<token_num && token_arr[i] != NULL) {
		printf("%s\n", token_arr[i]);
		i++;
	}
}

void prompt() {
	char cwd[CWDBUF];
	if(getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("%s", cwd);
		printf(" %% ");
	}
}
