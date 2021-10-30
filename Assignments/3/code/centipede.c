#include "centipede.h"

pthread_mutex_t screenMutex;
pthread_t screenThread;

void centipedeRun() {
	wrappedMutexInit(&screenMutex, NULL);	
	wrappedPthreadCreate(&screenThread, NULL, screenRefresh, NULL);
	
	if(consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD)) {
		int i = 0;
		while(i < 6) {
			char** ship = SHIP[i%2];
			wrappedMutexLock(&screenMutex);
			consoleClearImage(20, 38+i-1, SHIP_HEIGHT, SHIP_WIDTH);
			consoleDrawImage(20, 38+i, ship, SHIP_HEIGHT);
			wrappedMutexUnlock(&screenMutex);
			sleepTicks(10);
			i++;
		}
		finalKeypress();
	}
	consoleFinish();
}

void* screenRefresh(void* dummy) {
	while(true) {
		wrappedMutexLock(&screenMutex);
		consoleRefresh();
		wrappedMutexUnlock(&screenMutex);
		sleepTicks(1);
	}
}
