//bullet.h
//the .h file for bullet.c
#ifndef BULLET_H
#define BULLET_H

#include <pthread.h>
#include <stdbool.h>

//defines the two types of bullets
typedef enum bullet_type {
	PLAYER,
	ENEMY
} bulletType;

//holds all the information for a bullet
typedef struct bullet_struct {
	int row;
	int col;
	int prevRow;
	int velocity;
	bool isAlive;
	bool isJoined;
	bulletType type;
	pthread_t thread;
	pthread_mutex_t mutex;
} bullet;

//draw a blank space over the last place a bullet was drawn
void clearBullet(bullet* b);

//create a new bullet and start the bullet thread
bullet* spawnBullet(int startRow, int startCol, bulletType type);

//draw the bullet at its current position to the buffer
void drawBullet(bullet* b);

//move a bullet in a certain direction depending on bullet type
void moveBullet(bullet* b);

//the function passed to the thread for a bullet
void* runBullet(void* data);
#endif
