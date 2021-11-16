//enemy.h
//the .h file for enemy.c
#ifndef ENEMY_H
#define ENEMY_H

#include <pthread.h>
#include <stdbool.h>
#include "bullet.h"
#include "player.h"

#define ENEMY_HEAD_TILES 12
#define ENEMY_HEIGHT 1
#define ENEMY_WIDTH 4

typedef struct enemy_segment {
	struct enemy_segment* next;
	int row;
	int col;
	int prevRow;
	int prevCol;
	int direction;
	bool onScreen;
} segment;

typedef struct enemy_struct {
	segment* head;
	segment* tail;
	int length;
	bool isAlive;
	bool isJoined;
	int animTile;
	int ticks;
	pthread_t thread;
	pthread_mutex_t mutex;
} enemy;

typedef struct hit_struct {
	enemy* enemyHit;
	segment* segmentHit;
	segment* prevSegment;
} hit;

hit* createHit(enemy* e, segment* s, segment* prev);
hit* checkHit(bullet* b);
enemy* splitEnemy(hit* h);
bool enemyCollision(bullet* b, segment* s);
bool playerCollision(bullet* b, player* p);
void moveEnemy(enemy* e);
void drawEnemy(enemy* e);
void drawEnemySegments(enemy* e);
void nextEnemyAnim(enemy* e);
void* runEnemy(void* data);
void* runEnemySpawner(void* data);
enemy* spawnEnemy(int length);
void addSegment(enemy* e);

#endif
