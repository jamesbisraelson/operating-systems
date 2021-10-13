#ifndef CMDS_H
#define CMDS_H

void prompt();
void get_cmd_table(const int cmds_num, const int args_num, char* cmds_arr[cmds_num][args_num], char* str_in);
void tokenize_to_array(const int token_num, char* token_arr[token_num], char* str_in, char* delim);
void print_cmd_table(const int i_max, const int j_max, char* table[i_max][j_max]); 
void print_strings(const int string_num, char* string_arr[string_num]);
void run_cmd_pipeline(const int cmds_num, const int args_num, char* cmds_arr[cmds_num][args_num]);
#endif
