#include "gameglobals.h"

#include "console.h"
#include "threadwrappers.h"

pthread_mutex_t screenMutex;
pthread_mutex_t gameOverMutex;
pthread_mutex_t randomMutex;
pthread_mutex_t bulletListMutex;
pthread_mutex_t enemyListMutex;
pthread_mutex_t scoreMutex;

pthread_cond_t gameOverCond;

pthread_t screenThread;
pthread_t keyboardThread;
pthread_t spawnerThread;
pthread_t cleanupThread;

bulletList* bList;
enemyList* eList;
player* p;

bool gameOver = false;
int score;

char *GAME_BOARD[] = {
"Score:          Lives:",
"=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-centipiede!=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"................................................................................",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
""};

bool isGameOver() {
	wrappedMutexLock(&gameOverMutex);
	if(gameOver) {
		wrappedMutexUnlock(&gameOverMutex);
		return true;
	}
	wrappedMutexUnlock(&gameOverMutex);
	return false;
}

void endGame() {
	wrappedMutexLock(&gameOverMutex);	
	wrappedCondSignal(&gameOverCond);
	gameOver = true;
	wrappedMutexUnlock(&gameOverMutex);
}
