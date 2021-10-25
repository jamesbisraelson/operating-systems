/*
 * file: cmds.c
 * ------------
 * defines all of the functions to tokenize and run external commands
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "cmds.h"

#define PIPEDELIM "|"
#define SPACEDELIM " "

/*
 * function: tokenize_to_array
 * ---------------------------
 * splits a string into tokens and stores each token in an array (token_arr).
 *
 * token_num: the length of token_num (max amount of tokens to be split)
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

/*
 * function: get_cmd_table
 * -----------------------
 * takes in an string of commands piped together (using "|")
 * and splits it into a 2d array of strings (cmds_arr) where each row
 * is for new pipe and each column is the command/argument string.
 *
 * cmds_num: the max amount of commands in the string
 * args_num: the max amount of arguments per command
 * cmds_arr: the array in which to store the commands/arguments
 * str_in: the string to be split
 */
void get_cmd_table(const int cmds_num, const int args_num, char* cmds_arr[cmds_num][args_num], char* str_in) {
	char* token_arr[cmds_num];
	tokenize_to_array(cmds_num, token_arr, str_in, PIPEDELIM);

	int i = 0;
	while(i<cmds_num) {
		tokenize_to_array(args_num, cmds_arr[i], token_arr[i], SPACEDELIM);
		i++;
	}
}

/*
 * function: run_cmd_pipeline
 * --------------------------
 * takes in a 2d array of strings (representing commands to be piped together),
 * and creates the pipes, forks, execs the process, and then waits for it
 * to be completed
 *
 * cmds_num: the max amount of commands (height of cmds_arr)
 * args_num: the max amount of arguments (width of cmds_arr)
 * cmds_arr: the array in which the commands/arguments are stored
 */
int run_cmd_pipeline(const int cmds_num, const int args_num, char* cmds_arr[cmds_num][args_num]) {
	int pipefd[2];
	pid_t pid, wpid;
	int fd_in = STDIN_FILENO;
	int status = 0;
	int i = 0;
	while(cmds_arr[i][0] != NULL) {
		//create pipe
		if((pipe(pipefd)) == -1) {
			perror("pipe error");
			exit(1);
		}
		//fork
		if((pid=fork()) == -1) {
			perror("fork error");
			exit(1);
		}
		//if child, else parent
		if(pid == 0) {//child
			//set fd_in to stdin
			if((dup2(fd_in, STDIN_FILENO)) == -1) {
				perror("dup2 error");
				exit(1);
			}
			//if the last command, make sure it outputs to stdout
			if(cmds_arr[i+1][0] != NULL) {
				if((dup2(pipefd[1], STDOUT_FILENO)) == -1) {
					perror("dup2 error");
					exit(1);
				}
			}
			close(fd_in);
			//exec the command
			if((execvp(cmds_arr[i][0], cmds_arr[i])) == -1) {
				perror("exec error");
				exit(1);
			}
			exit(1);
		}
		else {//parent
			//set fd_in to the read end of the pipe so the next
			//iteration can use it
			close(pipefd[1]);
			fd_in = pipefd[0];
			i++;
		}
	}
	//wait until all the children are done
	while((wpid = waitpid(-1, &status, WUNTRACED)) > 0) {
		//check to see if child process is stopped and if so return the pid
		if(WIFSTOPPED(status)) {	
			printf("The job was suspended. Type 'fg' to resume.\n");
			return wpid;
		}
	}
	return 0;
}

