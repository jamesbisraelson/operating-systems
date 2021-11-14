#ifndef BULLET_H
#define BULLET_H

#include <pthread.h>
#include <stdbool.h>

typedef enum bullet_type {
	PLAYER,
	ENEMY
} bulletType;

typedef struct bullet_struct {
	int row;
	int col;
	int prevRow;
	int velocity;
	bool isAlive;
	bulletType type;
	pthread_t thread;
	pthread_mutex_t mutex;
} bullet;

void clearBullet(bullet* b);
bullet* spawnBullet(int startRow, int startCol, bulletType type);
void drawBullet(bullet* b);
void moveBullet(bullet* b);
void* runBullet(void* data);
#endif
