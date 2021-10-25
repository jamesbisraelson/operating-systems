/*
 * file: myshell.c
 * ---------------
 * defines the main function for myshell
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"
#include "printing.h"
#include "signals.h"

#define ARGNUM 100
#define CMDNUM 100
#define BUFSIZE 1023

int main(int argc, char* argv[]) {
	void catch_signal(int);
	signal(SIGTSTP, catch_signal);

	char* buf_in;
	char buffer[BUFSIZE];
	int error = 0;
	int sus_pid = 0;
	//int status;

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
		else if(!strcmp(cmds[0][0], "fg")) {
			if(sus_pid == 0) {
				printf("There is no job currently suspended.\n");
			}
			else {
				kill(sus_pid, SIGCONT);
				sus_pid = -1;
				wait(NULL);
			}
		}
		else if(!strcmp(cmds[0][0], "bg")) {
			if(sus_pid == 0) {
				printf("There is no job currently suspended\n");
			}
			else {
				kill(sus_pid, SIGCONT);
			}
		}

		//else run system commands
		else {
			if(sus_pid == 0) {
				sus_pid = run_cmd_pipeline(CMDNUM, ARGNUM, cmds);
			}
			else {
				printf("Not allowed to run a new command while you have a job active\n");
			}
		}
		prompt();
		buf_in = fgets(buffer, BUFSIZE, stdin);
	}
	return 0;
}
