#include <stdlib.h>
#include "bullet.h"
#include "console.h"
#include "threadwrappers.h"
#include "centipede.h"
#include "gameglobals.h"
#include "list.h"

#define BULLET_TICKS 5

char* BULLET_SPRITE[] = { "*" };

//helper function, not thread safe
static bool isInBounds(bullet* b) {
	if(b->row > LOWER_GAME_BOUND) return false;
	if(b->row < UPPER_GAME_BOUND) return false;
	return true;
}

static void setState(bullet* b) {
	wrappedMutexLock(&b->mutex);
	if(isGameOver()) {
		b->isAlive = false;
	}
	else if(!isInBounds(b)) {
		b->isAlive = false;
	}
	else if(checkHit(b)) {
		b->isAlive = false;
	}
	wrappedMutexUnlock(&b->mutex);
}

void* runBullet(void* data) {
	bullet* b = (bullet*)data;

	while(true) {
		setState(b);
		wrappedMutexLock(&b->mutex);
		if(!(b->isAlive)) {
			wrappedMutexUnlock(&b->mutex);
			clearBullet(b);
			pthread_exit(NULL);
		}
		wrappedMutexUnlock(&b->mutex);
		drawBullet(b);
		moveBullet(b);
		sleepTicks(BULLET_TICKS);
	}
}


void clearBullet(bullet* b) {
	wrappedMutexLock(&b->mutex);
	b->row = -1;
	wrappedMutexUnlock(&b->mutex);
	drawBullet(b);
}


bool checkHit(bullet* b) {
	int i;
	int j;
	wrappedMutexLock(&enemyListMutex);
	enemyNode* eNode = eList->head;
	for(i=0; i<eList->length; i++) {
		wrappedMutexLock(&eNode->payload->mutex);
		enemy* e = eNode->payload;
		segment* s = e->head;	
		for(j=0; j<e->length; j++) {
			if(e->isAlive == false) break;
			if(collision(b, s)) {
				e->isAlive = false;
				wrappedMutexUnlock(&e->mutex);
				wrappedMutexUnlock(&enemyListMutex);
				return true;
			}
			s = s->next;
		}
		wrappedMutexUnlock(&e->mutex);
		eNode = eNode->next;
	}
	wrappedMutexUnlock(&enemyListMutex);
	return false;
}

bool collision(bullet* b, segment* s) {
	if(abs(b->row - s->row) == 0) {
		if(abs(b->col - s->col) <= ENEMY_WIDTH) {
			return true;
		}
	}
	return false;
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

