#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "signal.h"
/*
int main(int argc, char* argv[]) {
	int status;
	void catch_signal(int);	
	char* count[] = {"./count", NULL};
	pid_t pid;
	
	signal(SIGTSTP, catch_signal);
	if((pid = fork()) == -1) {
		perror("fork error");
		exit(1);
	}
	if(pid == 0) {
		execvp(count[0], count);
	}
	pid = waitpid(-1, &status, WUNTRACED);
	if(WIFSTOPPED(status)) {
		kill(pid, SIGCONT);
	}
}*/

void catch_signal(int the_signal) {
	signal(the_signal, catch_signal);
	printf("CAUGHT IT\n");
}
