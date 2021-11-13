#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "centipede.h"
#include "threadwrappers.h"
#include "console.h"
#include "gameglobals.h"
#include "player.h"
#include "keyboard.h"
#include "bullet.h"
#include "enemy.h"

void centipedeRun() {
	if(consoleInit(LOWER_GAME_BOUND, RIGHT_GAME_BOUND, GAME_BOARD)) {
		wrappedMutexInit(&screenMutex, NULL);	
		wrappedMutexInit(&gameOverMutex, NULL);
		wrappedMutexInit(&randomMutex, NULL);
		wrappedMutexInit(&bulletListMutex, NULL);
		wrappedMutexInit(&enemyListMutex, NULL);
		
		bList = mallocBulletList();
		eList = mallocEnemyList();

		wrappedMutexLock(&randomMutex);
		srand(time(0));
		wrappedMutexUnlock(&randomMutex);

		player* p = spawnPlayer(22, 38, 3);
		wrappedPthreadCreate(&screenThread, NULL, screenRefresh, NULL);
		wrappedPthreadCreate(&keyboardThread, NULL, runKeyboard, p);
		wrappedPthreadCreate(&spawnerThread, NULL, runEnemySpawner, eList);

		wrappedMutexLock(&gameOverMutex);	
		wrappedCondWait(&gameOverCond, &gameOverMutex);
		wrappedMutexUnlock(&gameOverMutex);
		
		wrappedMutexLock(&screenMutex);
		finalKeypress();
		consoleFinish();
		wrappedMutexUnlock(&screenMutex);
		
		wrappedPthreadJoin(screenThread, NULL);
		wrappedPthreadJoin(p->thread, NULL);
		wrappedPthreadJoin(keyboardThread, NULL);
		wrappedPthreadJoin(spawnerThread, NULL);

		freeAndJoinEnemyList(eList);
		freeAndJoinBulletList(bList);
	}
}

void* screenRefresh(void* dummy) {
	while(true) {
		if(isGameOver()) {
			pthread_exit(NULL);
		}
		wrappedMutexLock(&screenMutex);
		consoleRefresh();
		wrappedMutexUnlock(&screenMutex);
		sleepTicks(1);
	}
}

bool isGameOver() {
	wrappedMutexLock(&gameOverMutex);
	if(gameOver) {
		wrappedMutexUnlock(&gameOverMutex);
		return true;
	}
	wrappedMutexUnlock(&gameOverMutex);
	return false;
}

