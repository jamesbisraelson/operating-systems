#ifndef ENEMY_H
#define ENEMY_H

#include <pthread.h>
#include <stdbool.h>

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
	int animTile;
	pthread_t thread;
	pthread_mutex_t mutex;
} enemy;

void moveEnemy(enemy* e);
void drawEnemy(enemy* e);
void drawEnemySegments(enemy* e);
void nextEnemyAnim(enemy* e);
void* runEnemy(void* data);
enemy* spawnEnemy(int length);
void addSegment(enemy* e);

#endif
