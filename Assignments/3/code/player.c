#include "player.h"

const char* SHIP[SHIP_ANIM_TILES][SHIP_HEIGHT] = {
{
	" /\\ ",
	" || ",
	"/--\\",
	" (  ",
	"  ) "
},
{
	" /\\ ",
	" || ",
	"/--\\",
	"  ) ",
	" (  "
}};

void* runPlayer(void* dummy) {
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
	wrappedMutexLock(&gameOverMutex);	
	wrappedCondSignal(&gameOverCond);
	gameOver = true;
	wrappedMutexUnlock(&gameOverMutex);
	pthread_exit(NULL);
	return NULL;
}

