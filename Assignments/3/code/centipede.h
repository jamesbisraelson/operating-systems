//centipede.h
//the .h file for centipede.c

#ifndef CENTIPEDE_H
#define CENTIPEDE_H

#include <pthread.h>
#include <stdbool.h>
#include "list.h"

//the main executing function for the game
void centipedeRun();

//thread to refresh screen
void* screenRefresh(void* dummy);

//thread to cleanup memory and check score/lives
void* runCleanup(void* dummy);

#endif
