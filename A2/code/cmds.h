#ifndef CMDS_H
#define CMDS_H

int run_cmds(char** arg_arr, const int arr_size);
void prompt();
void tokenize_to_array(char** array, const int arr_size, char* buffer, const char* delim);
#endif
