//bullet.c
//holds all the functions to run a bullet thread
#include <stdlib.h>
#include "bullet.h"
#include "console.h"
#include "threadwrappers.h"
#include "gameglobals.h"
#include "list.h"

//bullet speed
#define BULLET_TICKS 10

//bullet animation
char* BULLET_SPRITE[] = { "*" };

//checks to see if a bullet is in bounds
//helper function, not thread safe
static bool isInBounds(bullet* b) {
	if(b->row > LOWER_GAME_BOUND) return false;
	if(b->row < UPPER_GAME_BOUND) return false;
	return true;
}

//checks to see if a bullet should be alive and if not sets it to dead
static void setState(bullet* b) {
	hit* h;
	//if game is over set to dead
	if(isGameOver()) {
		b->isAlive = false;
	}
	//if out of bounds set to dead
	else if(!isInBounds(b)) {
		b->isAlive = false;
	}
	//if bullet hits an enemy set to dead
	else if((h = checkHit(b)) != NULL) {
		b->isAlive = false;
		addEnemy(eList, splitEnemy(h));
		free(h);
	}
	//if player is dead set bullet to dead
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
		
		//if bullet is dead, exit thread
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
	//set bullet off screen and redraw
	b->row = -1;
	drawBullet(b);
}


void moveBullet(bullet* b) {
	b->row += b->velocity;
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
	//create bullet
	bullet* b = malloc(sizeof(bullet));
	addBullet(bList, b);
	
	b->row = startRow;
	b->col = startCol;
	b->prevRow = b->row;
	b->type = type;
	b->isAlive = true;
	b->isJoined = false;
	
	//set velocity based on type
	if(b->type == PLAYER) {
		b->velocity = -1;
	}
	else if(b->type == ENEMY) {
		b->velocity = 1;
	}
	
	//start thread
	wrappedMutexInit(&b->mutex, NULL);
	wrappedPthreadCreate(&b->thread, NULL, runBullet, (void*)b);
	return b;
}

