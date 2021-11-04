#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <pthread.h>

extern pthread_mutex_t screenMutex;
extern pthread_mutex_t gameOverMutex;
extern pthread_cond_t gameOverCond;
void centipedeRun();
void* screenRefresh(void* dummy);

#endif