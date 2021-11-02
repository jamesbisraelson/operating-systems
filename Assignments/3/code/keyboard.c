#include "keyboard.h"
#include "player.h"
#include "centipede.h"
#include "threadwrappers.h"
#include "gameglobals.h"
#include "console.h"
#include <time.h>
#include <stdlib.h>

void* runKeyboard(void* data) {
	player* p = (player*)data;
	srand(time(NULL));
	
	int i;
	for(i=0; i<50; i++) {
		wrappedMutexLock(&(p->mutex));
		movePlayer(p, -1*rand()%2, rand()%2);
		wrappedMutexUnlock(&(p->mutex));
		sleepTicks(10);
	}
	wrappedMutexLock(&gameOverMutex);	
	wrappedCondSignal(&gameOverCond);
	gameOver = true;
	wrappedMutexUnlock(&gameOverMutex);
	pthread_exit(NULL);
}
