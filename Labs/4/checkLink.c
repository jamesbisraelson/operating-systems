#include <stdio.h>
#define BUFSIZE 32768
int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("the program needs one argument\n");
	}
	
	char* buf[BUFSIZE+1];
	if(readlink(argv[1], buf, BUFSIZE) == -1) {
		perror("readlink error");
	}
	printf("%s", buf);
}
