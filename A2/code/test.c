#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define PIPEDELIM "|"
#define SPACEDELIM " "


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

void get_commands(const int cmds_num, const int args_num, char* cmds_arr[cmds_num][args_num], char* str_in) {
	char* token_arr[cmds_num];
	tokenize_to_array(cmds_num, token_arr, str_in, PIPEDELIM);
	
	int i = 0;
	while(i<cmds_num && token_arr[i] != NULL) {
		tokenize_to_array(args_num, cmds_arr[i], token_arr[i], SPACEDELIM);
		i++;
	}
}

int main(int arc, char* argv[]) {
	char command[] = "ls -A | grep c";
	
	char* cmds_arr[10][10];
	get_commands(10, 10, cmds_arr, command);


	int pipefd[2];
	pid_t p1, p2;

	if(pipe(pipefd) < 0) {
		perror("pipe error");
	}

	p1 = fork();
	if(p1 < 0) {//child
		perror("fork 1 error");
	}

	if(p1 == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		
		if(execvp(cmds_arr[0][0], cmds_arr[0]) < 0) {
			perror("exec error");
			exit(0);
		}
	}
	else {//parent
		p2 = fork();
		if(p2<0) {
			perror("fork 2 error");
		}

		if(p2 == 0) {//child
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			
			if(execvp(cmds_arr[1][0], cmds_arr[1]) < 0) {
				perror("exec error");
				exit(0);
			}
		}
		else {//parent
			close(pipefd[0]);
			close(pipefd[1]);
			wait(NULL);
			wait(NULL);
		}
	}

	return 0;
}
