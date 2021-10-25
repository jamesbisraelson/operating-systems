/*
 * file: printing.h
 * ------------
 * the associated .h file for printing.c
 */
#ifndef PRINTING_H
#define PRINTING_H

void print_cmd_table(const int i_max, const int j_max, char* table[i_max][j_max]);
void print_strings(const int string_num, char* string_arr[string_num]);
void prompt();

#endif
