#include <stdlib.h>
#include <pthread.h>

#include "centipede.h"
#include "threadwrappers.h"
#include "console.h"
#include "gameglobals.h"
#include "player.h"
#include "keyboard.h"
#include "bullet.h"

pthread_mutex_t screenMutex;
pthread_mutex_t gameOverMutex;
pthread_cond_t gameOverCond;
pthread_t screenThread;
pthread_t keyboardThread;
bulletList* bList;

void centipedeRun() {

	if(consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD)) {
		bList = mallocBulletList();
		wrappedMutexInit(&screenMutex, NULL);	
		wrappedMutexInit(&gameOverMutex, NULL);
		
		player* p = spawnPlayer(22, 38, 3);
		wrappedPthreadCreate(&screenThread, NULL, screenRefresh, NULL);
		wrappedPthreadCreate(&keyboardThread, NULL, runKeyboard, p);

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
		
		int i;
		bulletNode* node = bList->head;
		for(i=0; i<bList->length; i++) {
			wrappedPthreadJoin(node->payload->thread, NULL);
			free(node->payload);
			
			bulletNode* toFree = node;
			node = node->next;

			toFree->next = NULL;
			free(toFree);
		}
	}
}

void* screenRefresh(void* dummy) {
	while(true) {
		wrappedMutexLock(&gameOverMutex);
		if(gameOver) {
			wrappedMutexUnlock(&gameOverMutex);
			pthread_exit(NULL);
		}
		wrappedMutexUnlock(&gameOverMutex);
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

