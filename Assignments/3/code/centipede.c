#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>

#include "centipede.h"
#include "bullet.h"
#include "threadwrappers.h"
#include "console.h"
#include "gameglobals.h"
#include "player.h"
#include "keyboard.h"
#include "enemy.h"

#define START_COL 38
#define START_ROW 20

#define SCORE_COL 7
#define SCORE_ROW 0
#define SCORE_LEN 10

#define LIVES 3
#define LIVES_COL 23
#define LIVES_ROW 0
#define LIVES_LEN 2

void centipedeRun() {
	if(consoleInit(LOWER_GAME_BOUND, RIGHT_GAME_BOUND, GAME_BOARD)) {
		wrappedMutexInit(&screenMutex, NULL);	
		wrappedMutexInit(&gameOverMutex, NULL);
		wrappedMutexInit(&randomMutex, NULL);
		wrappedMutexInit(&bulletListMutex, NULL);
		wrappedMutexInit(&enemyListMutex, NULL);
		wrappedMutexInit(&scoreMutex, NULL);
	
		score = 0;

		bList = mallocBulletList();
		eList = mallocEnemyList();

		wrappedMutexLock(&randomMutex);
		srand(time(0));
		wrappedMutexUnlock(&randomMutex);
		
		p = spawnPlayer(START_ROW, START_COL, LIVES);
		wrappedMutexLock(&p->mutex);
		wrappedPthreadCreate(&screenThread, NULL, screenRefresh, NULL);
		wrappedPthreadCreate(&keyboardThread, NULL, runKeyboard, p);
		wrappedMutexUnlock(&p->mutex);
		wrappedPthreadCreate(&spawnerThread, NULL, runEnemySpawner, eList);
		wrappedPthreadCreate(&cleanupThread, NULL, runCleanup, NULL);

		wrappedMutexLock(&gameOverMutex);	
		wrappedCondWait(&gameOverCond, &gameOverMutex);
		wrappedMutexUnlock(&gameOverMutex);
	

		wrappedMutexLock(&screenMutex);
		finalKeypress();
		consoleFinish();
		wrappedMutexUnlock(&screenMutex);
		
		wrappedPthreadJoin(screenThread, NULL);
		wrappedPthreadJoin(p->thread, NULL);
		free(p);
		wrappedPthreadJoin(keyboardThread, NULL);
		wrappedPthreadJoin(cleanupThread, NULL);
		
		joinEnemyList(eList);
		freeEnemyList(eList);
		joinBulletList(bList);
		freeBulletList(bList);
		
		//this thread takes a while to clean up because it has a very long sleep timer
		wrappedPthreadJoin(spawnerThread, NULL);
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

void* runCleanup(void* dummy) {
	while(true) {
		if(isGameOver()) {
			pthread_exit(NULL);
		}
		wrappedMutexLock(&enemyListMutex);
		wrappedMutexLock(&scoreMutex);
		if(eList->liveEnemies == 0 && score > 0) {
			wrappedMutexUnlock(&enemyListMutex);
			wrappedMutexUnlock(&scoreMutex);
			endGame();
			pthread_exit(NULL);
		}
		wrappedMutexUnlock(&enemyListMutex);
		wrappedMutexUnlock(&scoreMutex);
		joinEnemyList(eList);
		joinBulletList(bList);
		
		char score_str[SCORE_LEN];
		char lives_str[LIVES_LEN];
		
		sprintf(score_str, "%d", score);
		
		wrappedMutexLock(&p->mutex);
		sprintf(lives_str, "%d", p->lives);
		wrappedMutexUnlock(&p->mutex);
		
		wrappedMutexLock(&screenMutex);
		putString(score_str, SCORE_ROW, SCORE_COL, SCORE_LEN);
		putString(lives_str, LIVES_ROW, LIVES_COL, LIVES_LEN);
		wrappedMutexUnlock(&screenMutex);

		sleepTicks(5);
	}
}
