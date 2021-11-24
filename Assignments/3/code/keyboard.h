//keyboard.h
//the .h file for keyboard.c
#ifndef KEYBOARD_H
#define KEYBOARD_H

//check to see if there is input available and return true if so
int kbhit();

//the function to run the keyboard thread
void* runKeyboard(void* data);
#endif
