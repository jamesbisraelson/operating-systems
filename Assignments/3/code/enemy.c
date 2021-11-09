#include <stdlib.h>
#include "enemy.h"
#include "gameglobals.h"
#include "threadwrappers.h"
#include "centipede.h"
#include "console.h"

#define ENEMY_TICKS 20

//i apologize for what i have created
//but this assignment was hard so you deserve it
char* ENEMY_HEAD[ENEMY_HEAD_TILES][ENEMY_HEIGHT] = {
	{"OwwO"},
	{"owwO"},
	{"owwO"},
	{"owwO"},
	{"OwwO"},
	{"OwwO"},
	{"OwwO"},
	{"Owwo"},
	{"Owwo"},
	{"Owwo"},
	{"OwwO"},
	{"OwwO"}
};

char* ENEMY_BODY[] = { "()()" };

void* runEnemy(void* data) {
	enemy* e = (enemy*)data;
	while(true) {
		drawEnemy(e);
		moveEnemy(e);
		
		nextEnemyAnim(e);	
		wrappedMutexLock(&gameOverMutex);
		if(gameOver) {
			wrappedMutexUnlock(&gameOverMutex);
			pthread_exit(NULL);
		}
		wrappedMutexUnlock(&gameOverMutex);
		sleepTicks(ENEMY_TICKS);
	}
}

void moveEnemy(enemy* e) {
	int i;
	wrappedMutexLock(&e->mutex);
	segment* s = e->head;
	for(i=0; i<e->length; i++) {
		if(s->onScreen) {
			//check to see if segment has hit a wall
			//let animation overlap into wall by half the
			//width of the enemy
			if(s->col < LEFT_GAME_BOUND - ENEMY_WIDTH/2) {
				s->row++;
				s->direction = RIGHT;
			}
			else if(s->col > RIGHT_GAME_BOUND - ENEMY_WIDTH/2) {
				s->row++;
				s->direction = LEFT;
			}
		}

		//move segment according to its direction
		if(s->direction == RIGHT) {
			s->col++;
		}
		else {
			s->col--;
		}

		if(!s->onScreen && s->col <= RIGHT_GAME_BOUND - ENEMY_WIDTH) {
			s->onScreen = true;
		}
		s = s->next;
	}
	wrappedMutexUnlock(&e->mutex);
}

void nextEnemyAnim(enemy* e) {
	wrappedMutexLock(&e->mutex);
	e->animTile++;
	e->animTile %= ENEMY_HEAD_TILES;
	wrappedMutexUnlock(&e->mutex);
}

void drawEnemy(enemy* e) {
	wrappedMutexLock(&screenMutex);
	wrappedMutexLock(&e->mutex);
	consoleClearImage(e->head->prevRow, e->head->prevCol, ENEMY_HEIGHT, ENEMY_WIDTH);
	consoleDrawImage(e->head->row, e->head->col, ENEMY_HEAD[e->animTile], ENEMY_HEIGHT);
	
	drawEnemySegments(e);
	wrappedMutexUnlock(&screenMutex);

	e->head->prevRow = e->head->row;
	e->head->prevCol = e->head->col;
	wrappedMutexUnlock(&e->mutex);
}

//not thread safe
void drawEnemySegments(enemy* e) {
	int i;
	//do not print the head
	segment* s = e->head->next;
	for(i=1; i<e->length; i++) {
		consoleClearImage(s->prevRow, s->prevCol, ENEMY_HEIGHT, ENEMY_WIDTH);
		consoleDrawImage(s->row, s->col, ENEMY_BODY, ENEMY_HEIGHT);
		
		s->prevRow = s->row;
		s->prevCol = s->col;
		s = s->next;
	}
}

enemy* spawnEnemy(int length) {
	enemy* e = malloc(sizeof(enemy));

	e->length = 0;
	e->isAlive = true;
	e->animTile = 0;

	int i;
	for(i=0; i<length; i++) {
		addSegment(e);
	}

	wrappedMutexInit(&e->mutex, NULL);
	wrappedPthreadCreate(&e->thread, NULL, runEnemy, (void*)e);
	
	return e;
}

void addSegment(enemy* e) {
	segment* s = malloc(sizeof(segment));
	if(e->length == 0) {
		s->row = UPPER_GAME_BOUND;
		s->col = RIGHT_GAME_BOUND;
		e->head = s;
		e->tail = s;
	}
	else {
		s->row = e->tail->row;
		s->col = e->tail->col + ENEMY_WIDTH;
		e->tail->next = s;
		e->tail = s;
	}
	s->onScreen = false;
	s->next = NULL;
	s->prevRow = s->row;
	s->prevCol = s->col;
	s->direction = LEFT;
	e->length++;
}
