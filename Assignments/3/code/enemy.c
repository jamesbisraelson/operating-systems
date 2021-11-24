//enemy.c
//holds all the functions to run the enemy thread

#include <stdlib.h>
#include "gameglobals.h"
#include "threadwrappers.h"
#include "console.h"

#define SHOOT_TICKS 50
#define HIT_SCORE 10
#define ENEMY_SCORE 100
#define ENEMY_MIN 1
#define ENEMY_LENGTH 8
#define ENEMY_TICKS 10
#define SPAWNER_TICKS_MIN 400
#define SPAWNER_TICKS_MAX 600
#define ENEMY_DEAD_SLEEP 20

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

static void setState(enemy* e) {
	if(isGameOver()) {
		e->isAlive = false;
	}
	else if(e->length <= ENEMY_MIN) {
		e->isAlive = false;
	}
}

void* runEnemy(void* data) {
	enemy* e = (enemy*)data;
	int shootCounter = 0;

	while(true) {
		wrappedMutexLock(&e->mutex);
		setState(e);
		if(!(e->isAlive)) {	
			wrappedMutexLock(&scoreMutex);
			score += ENEMY_SCORE;
			wrappedMutexUnlock(&scoreMutex);
			wrappedMutexUnlock(&e->mutex);
			pthread_exit(NULL);
		}
		wrappedMutexLock(&p->mutex);
		if(p->state == DEAD) {
			sleepTicks(ENEMY_DEAD_SLEEP);
		}
		wrappedMutexUnlock(&p->mutex);
		
		wrappedMutexLock(&randomMutex);
		shootCounter = rand() % SHOOT_TICKS;
		wrappedMutexUnlock(&randomMutex);
		if(shootCounter == 0) {
			spawnBullet(e->head->row, e->head->col, ENEMY);
		}
		drawEnemy(e);
		moveEnemy(e);
		nextEnemyAnim(e);	
		
		wrappedMutexUnlock(&e->mutex);
		wrappedMutexLock(&e->mutex);
		int ticks = e->ticks;
		wrappedMutexUnlock(&e->mutex);
		sleepTicks(ticks);
	}
}

void* runEnemySpawner(void* data) {
	enemyList* el = (enemyList*)data;
	while(true) {
		if(isGameOver()) {
			pthread_exit(NULL);
		}
		wrappedMutexLock(&p->mutex);
		if(p->state == DEAD) {
			sleepTicks(ENEMY_DEAD_SLEEP);
		}
		wrappedMutexUnlock(&p->mutex);
		addEnemy(el, spawnEnemy(ENEMY_LENGTH));
		int ticks = rand() % SPAWNER_TICKS_MAX;
		ticks += SPAWNER_TICKS_MIN;
		sleepTicks(ticks);
	}
}

//not thread safe for bullets
hit* checkHit(bullet* b) {
	if(b->type == PLAYER) {
		int i;
		int j;
		wrappedMutexLock(&enemyListMutex);
		enemyNode* eNode = eList->head;
		for(i=0; i<eList->length; i++) {
			wrappedMutexLock(&eNode->payload->mutex);
			enemy* e = eNode->payload;
			segment* s = e->head;
			segment* prev = NULL;
			for(j=0; j<e->length; j++) {
				if(e->isAlive == false) break;
				if(enemyCollision(b, s)) {
					hit* h = createHit(e, s, prev);
					wrappedMutexUnlock(&e->mutex);
					wrappedMutexUnlock(&enemyListMutex);
					wrappedMutexLock(&scoreMutex);
					score += HIT_SCORE;
					wrappedMutexUnlock(&scoreMutex);
					return h;
				}
				prev = s;
				s = s->next;
			}
			wrappedMutexUnlock(&e->mutex);
			eNode = eNode->next;
		}
		wrappedMutexUnlock(&enemyListMutex);
	}
	else if(b->type == ENEMY) {
		wrappedMutexLock(&p->mutex);
		if(playerCollision(b, p) && p->state == GAME) {
			b->isAlive = false;
			if(p->lives == 0) {
				p->state = GAMEOVER;
			}
			else {
				p->state = DEAD;
			}
			p->lives--;
		}
		wrappedMutexUnlock(&p->mutex);
	}
	return NULL;
}

bool playerCollision(bullet* b, player* p) {
	if(b->row < SHIP_HEIGHT + p->row &&
		b->row > p->row &&
		b->col < SHIP_WIDTH + p->col &&
		b->col >= p->col) {
		return true;
	}
	
	return false;
}

//not thread safe
bool enemyCollision(bullet* b, segment* s) {
	if(abs(b->row - s->row) == 0) {
		if(abs(b->col - s->col) <= ENEMY_WIDTH) {
			return true;
		}
	}
	return false;
}

static int getLength(enemy* e) {
	int length = 0;
	segment* current = e->head;
	while(current != NULL) {
		length++;
		current = current->next;
	}
	return length;
}

//not thread safe
hit* createHit(enemy* e, segment* s, segment* prev) {
	hit* h = malloc(sizeof(hit));
	h->enemyHit = e;
	h->segmentHit = s;
	h->prevSegment = prev;
	return h;
}

enemy* splitEnemy(hit* h) {
	//make current enemy shorter
	wrappedMutexLock(&h->enemyHit->mutex);

	segment* newHead = NULL;
	if(h->prevSegment == NULL) {
		h->enemyHit->tail = h->segmentHit;
		newHead = h->segmentHit->next;	
		h->segmentHit->next = NULL;
	}
	else {
		h->enemyHit->tail = h->prevSegment;
		newHead = h->segmentHit;
		h->prevSegment->next = NULL;
	}
	h->enemyHit->length = getLength(h->enemyHit);

	//create new enemy and return it
	enemy* e = malloc(sizeof(enemy));
	e->head = newHead;
	e->tail = h->enemyHit->tail;
	e->length = getLength(e);
	e->isAlive = true;
	e->isJoined = false;
	e->animTile = 0;
	e->ticks = h->enemyHit->ticks;
	wrappedMutexInit(&e->mutex, NULL);
	wrappedPthreadCreate(&e->thread, NULL, runEnemy, (void*)e);
	
	//set original enemy to faster speed
	h->enemyHit->ticks--;
	wrappedMutexUnlock(&h->enemyHit->mutex);
	
	return e;
}

void moveEnemy(enemy* e) {
	int i;
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
			//if enemy is at the bottom of the screen, do not move down one row
			if(s->row >= UPPER_PLAYER_BOUND - ENEMY_HEIGHT) {
				s->row--;
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
}

void nextEnemyAnim(enemy* e) {
	e->animTile++;
	e->animTile %= ENEMY_HEAD_TILES;
}

void drawEnemy(enemy* e) {
	wrappedMutexLock(&screenMutex);
	consoleClearImage(e->head->prevRow, e->head->prevCol, ENEMY_HEIGHT, ENEMY_WIDTH);
	consoleDrawImage(e->head->row, e->head->col, ENEMY_HEAD[e->animTile], ENEMY_HEIGHT);
	
	drawEnemySegments(e);
	wrappedMutexUnlock(&screenMutex);

	e->head->prevRow = e->head->row;
	e->head->prevCol = e->head->col;
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
	e->ticks = ENEMY_TICKS;
	e->isJoined = false;

	int i;
	for(i=0; i<length; i++) {
		addSegment(e);
	}

	wrappedMutexInit(&e->mutex, NULL);
	wrappedMutexLock(&e->mutex);
	wrappedPthreadCreate(&e->thread, NULL, runEnemy, (void*)e);
	wrappedMutexUnlock(&e->mutex);
	
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
