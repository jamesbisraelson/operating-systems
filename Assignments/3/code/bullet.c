#include <stdlib.h>
#include "bullet.h"
#include "console.h"
#include "threadwrappers.h"
#include "centipede.h"

#define BULLET_TICKS 5

char* BULLET_SPRITE[] = { "*" };

void* runBullet(void* data) {
	bullet* b = (bullet*)data;
	newBullet(b);

	while(true) {
		drawBullet(b);
		moveBullet(b);
		if(isGameOver()) {
			free(b);
			pthread_exit(NULL);
		}
		sleepTicks(BULLET_TICKS);
	}
	return 0;
}

void moveBullet(bullet* b) {
	wrappedMutexLock(&b->mutex);
	b->row += b->velocity;
	wrappedMutexUnlock(&b->mutex);
}

void drawBullet(bullet* b) {
	wrappedMutexLock(&screenMutex);
	wrappedMutexLock(&b->mutex);
	//clear the previous image (bullet size is 1x1)
	consoleClearImage(b->prevRow, b->col, 1, 1);
	//draw bullet
	consoleDrawImage(b->row, b->col, BULLET_SPRITE, 1);
	//set prevRow to the row that was just drawn
	b->prevRow = b->row;
	wrappedMutexUnlock(&screenMutex);
	wrappedMutexUnlock(&b->mutex);
}

void newBullet(bullet* b) {
	wrappedMutexLock(&b->mutex);
	switch(b->type) {
		case PLAYER:
			b->velocity = 1;
		case ENEMY:
			b->velocity = -1;
	}
	wrappedMutexUnlock(&b->mutex);
}

bullet* spawnBullet(int startRow, int startCol, bulletType type) {
	bullet* b = malloc(sizeof(bullet));
	b->row = startRow;
	b->col = startCol;
	b->prevRow = b->row;
	b->type = type;
	b->isAlive = true;
	wrappedMutexInit(&b->mutex, NULL);
	wrappedPthreadCreate(&b->thread, NULL, runBullet, (void*)b);
	return b;
}

