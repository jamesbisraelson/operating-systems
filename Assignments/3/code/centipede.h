#ifndef CENTIPEDE_H
#define CENTIPEDE_H

#include <pthread.h>
#include <stdbool.h>
#include "list.h"

void centipedeRun();
void* screenRefresh(void* dummy);
bool isGameOver();

#endif
