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

//this is a piece of an enemy
//an enemy is basically a linked list that can be split
typedef struct enemy_segment {
	struct enemy_segment* next;
	int row;
	int col;
	int prevRow;
	int prevCol;
	int direction;
	bool onScreen;
} segment;

//holds all the data for an enemy
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

//a struct holding the information for a bullet hi
typedef struct hit_struct {
	enemy* enemyHit;
	segment* segmentHit;
	segment* prevSegment;
} hit;

//creates a hit
hit* createHit(enemy* e, segment* s, segment* prev);

//checks to see if a bullet has hit an enemy or player
hit* checkHit(bullet* b);

//splits an enemy where it was hit
enemy* splitEnemy(hit* h);

//checks to see if a bullet is touching a segment
bool enemyCollision(bullet* b, segment* s);

//checks to see if a bullet is touching a player
bool playerCollision(bullet* b, player* p);

//moves an enemy
void moveEnemy(enemy* e);

//draws an enemy
void drawEnemy(enemy* e);

//draws a segment
void drawEnemySegments(enemy* e);

//incremements the enemy animation
void nextEnemyAnim(enemy* e);

//the function to run enemy thread
void* runEnemy(void* data);

//the function to run the enemy spawner
void* runEnemySpawner(void* data);

//creates an enemy and returns it
enemy* spawnEnemy(int length);

//adds a segment to an enemy
void addSegment(enemy* e);

#endif
