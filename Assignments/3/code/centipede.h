#ifndef CENTIPEDE_H
#define CENTIPEDE_H

#include <pthread.h>
#include <stdbool.h>
#include "list.h"

extern pthread_mutex_t screenMutex;
extern pthread_mutex_t gameOverMutex;
extern pthread_cond_t gameOverCond;
extern bulletList* bList;

void centipedeRun();
void* screenRefresh(void* dummy);
bool isGameOver();

#endif
