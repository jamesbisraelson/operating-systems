#include <stdlib.h>
#include "bullet.h"
#include "console.h"
#include "threadwrappers.h"
#include "gameglobals.h"
#include "list.h"

#define BULLET_TICKS 10

char* BULLET_SPRITE[] = { "*" };

//helper function, not thread safe
static bool isInBounds(bullet* b) {
	if(b->row > LOWER_GAME_BOUND) return false;
	if(b->row < UPPER_GAME_BOUND) return false;
	return true;
}

static void setState(bullet* b) {
	hit* h;
	if(isGameOver()) {
		b->isAlive = false;
	}
	else if(!isInBounds(b)) {
		b->isAlive = false;
	}
	else if((h = checkHit(b)) != NULL) {
		b->isAlive = false;
		addEnemy(eList, splitEnemy(h));
		free(h);
	}
	wrappedMutexLock(&p->mutex);
	if(p->state == DEAD) {
		b->isAlive = false;
	}
	wrappedMutexUnlock(&p->mutex);
}

void* runBullet(void* data) {
	bullet* b = (bullet*)data;

	while(true) {
		wrappedMutexLock(&b->mutex);
		setState(b);
		if(!(b->isAlive)) {
			clearBullet(b);
			wrappedMutexUnlock(&b->mutex);
			pthread_exit(NULL);
		}
		drawBullet(b);
		moveBullet(b);
		wrappedMutexUnlock(&b->mutex);
		sleepTicks(BULLET_TICKS);
	}
}


void clearBullet(bullet* b) {
	b->row = -1;
	drawBullet(b);
}

void moveBullet(bullet* b) {
	if(b->type == PLAYER) {
		b->row += b->velocity;
	}
	else if(b->type == ENEMY) {
		b->row += b->velocity;
	}
}

void drawBullet(bullet* b) {
	wrappedMutexLock(&screenMutex);
	//clear the previous image (bullet size is 1x1)
	consoleClearImage(b->prevRow, b->col, 1, 1);
	//draw bullet
	consoleDrawImage(b->row, b->col, BULLET_SPRITE, 1);
	//set prevRow to the row that was just drawn
	b->prevRow = b->row;
	wrappedMutexUnlock(&screenMutex);
}

bullet* spawnBullet(int startRow, int startCol, bulletType type) {
	bullet* b = malloc(sizeof(bullet));
	addBullet(bList, b);
	
	b->row = startRow;
	b->col = startCol;
	b->prevRow = b->row;
	b->type = type;
	b->isAlive = true;
	b->isJoined = false;

	if(b->type == PLAYER) {
		b->velocity = -1;
	}
	else if(b->type == ENEMY) {
		b->velocity = 1;
	}
	
	wrappedMutexInit(&b->mutex, NULL);
	wrappedPthreadCreate(&b->thread, NULL, runBullet, (void*)b);
	return b;
}

