#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cmds.h"

int main(int arc, char* argv[]) {
	char* gay[100][100];
	gay[0][0] = "penis";
	gay[0][1] = "nice";
	printf("%s", gay[0][0]);
	return 0;
}	
