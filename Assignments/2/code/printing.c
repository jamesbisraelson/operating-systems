/*
 * file: printing.c
 * ------------
 * defines all of the functions for myshell that print to stdout
 * some of these are only for testing purposes
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "printing.h"

#define CWDBUF 256

/*
 * function: print_cmd_table
 * -------------------------
 * prints out the contents of a command table (see get_cmd_table())
 *
 * i_max, j_max; the dimensions of table
 * table: the command table to be printed
 */
void print_cmd_table(const int i_max, const int j_max, char* table[i_max][j_max]) {
	int i = 0;
	while(i<i_max && table[i] != NULL) {
		print_strings(j_max, table[i]);
		i++;
	}
}

/*
 * function: print_tokens
 * ----------------------
 * a helper function for print_cmd_table. this function prints every item
 * in an array of strings
 *
 * string_num: the number of strings in string_arr
 * string_arr: the array of strings to be printed
 */
void print_strings(const int string_num, char* string_arr[string_num]) {
	int i = 0;
	while(i<string_num && string_arr[i] != NULL) {
		printf("%s\n", string_arr[i]);
		i++;
	}
}

/*
 * function: prompt
 * ----------------
 * gets the current working directory and prints it with a "%" following
 */
void prompt() {
	char cwd[CWDBUF];
	if(getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("%s", cwd);
		printf(" %% ");
	}
}
