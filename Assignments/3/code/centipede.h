#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <pthread.h>
#include "console.h"
#include "gameglobals.h"
#include "player.h"

extern pthread_mutex_t screenMutex;
void centipedeRun();
void* screenRefresh(void* dummy);

#endif
