#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmds.h"

int run_cmds(char* str_in) {
	char* token;
	char* save_ptr;
	char* delim = " ";

	token = strtok_r(str_in, delim, &save_ptr);
	while(token != NULL) {
		str_in = NULL;
		printf("%s\n", token);
		token = strtok_r(str_in, delim, &save_ptr);
	}
	return 0;
}
