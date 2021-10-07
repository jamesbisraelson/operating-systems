#ifndef CMDS_H
#define CMDS_H

int run_cmds_recursive(int index, int pipein, const int cmds_num, char* cmds_arr[], const int args_num, char* delim);
int run_cmds(const int cmds_num, char* cmds_arr[], const int args_num, char* delim);
void prompt();
void tokenize_to_array(const int token_num, char* token_arr[token_num], char* str_in, char* delim);
void printtokens(const int token_num, char* token_arr[token_num]);
#endif
