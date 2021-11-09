#include <stdlib.h>
#include "bullet.h"
#include "console.h"
#include "threadwrappers.h"
#include "centipede.h"
#include "gameglobals.h"

#define BULLET_TICKS 5

char* BULLET_SPRITE[] = { "*" };

//helper function, not thread safe
static bool isInBounds(bullet* b) {
	if(b->row > LOWER_GAME_BOUND) return false;
	if(b->row < UPPER_GAME_BOUND) return false;
	return true;
}

void* runBullet(void* data) {
	bullet* b = (bullet*)data;

	while(true) {
		drawBullet(b);
		moveBullet(b);
		setState(b);
		if(!(b->isAlive)) {
			clearBullet(b);
			pthread_exit(NULL);
		}
		sleepTicks(BULLET_TICKS);
	}
}

void clearBullet(bullet* b) {
	wrappedMutexLock(&b->mutex);
	b->row = -1;
	wrappedMutexUnlock(&b->mutex);
	drawBullet(b);
}

void setState(bullet* b) {
	if(!isInBounds(b)) {
		wrappedMutexLock(&b->mutex);
		b->isAlive = false;
		wrappedMutexUnlock(&b->mutex);
	}
	else if(isGameOver()) {
		wrappedMutexLock(&b->mutex);
		b->isAlive = false;
		wrappedMutexUnlock(&b->mutex);
	}
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

bullet* spawnBullet(int startRow, int startCol, bulletType type) {
	bullet* b = malloc(sizeof(bullet));
	addBullet(bList, b);
	
	b->row = startRow;
	b->col = startCol;
	b->prevRow = b->row;
	b->type = type;
	b->isAlive = true;
	
	switch(b->type) {
		case PLAYER:
			b->velocity = 1;
		case ENEMY:
			b->velocity = -1;
	}
	
	wrappedMutexInit(&b->mutex, NULL);
	wrappedPthreadCreate(&b->thread, NULL, runBullet, (void*)b);
	return b;
}

