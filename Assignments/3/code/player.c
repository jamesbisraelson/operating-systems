#include <stdlib.h>
#include "player.h"
#include "threadwrappers.h"
#include "console.h"
#include "gameglobals.h"
#include "centipede.h"
#include "bullet.h"

#define START_COL 20
#define START_ROW 38
#define PLAYER_TICKS 10

char* SHIP[SHIP_ANIM_TILES][SHIP_HEIGHT] = {
{
	"/-\\",
	"o  ",
	"  o"
},
{
	"/-\\",
	"  o",
	"o  "
}};

void shootBullet(player* p) {
	int bulletRow = p->row - 1;
	//align the bullet to the front of the ship
	int bulletCol = p->col + SHIP_WIDTH / 2;
	spawnBullet(bulletRow, bulletCol, PLAYER);
}

void* runPlayer(void* data) {
	player* p = (player*)data;
	newPlayer(p);

	while(true) {
		//draw the player and switch to the next animation tile
		drawPlayer(p);
		nextPlayerAnim(p);
		
		//if the game is over, free the player memory
		if(isGameOver()) {
			free(p);
			pthread_exit(NULL);
		}
		sleepTicks(PLAYER_TICKS);		
	}
}

void nextPlayerAnim(player* p) {
	wrappedMutexLock(&p->mutex);
	//increment player tile # 
	//but make sure it does not exceed the max amount of tiles
	p->animTile++;
	p->animTile %= SHIP_ANIM_TILES;
	wrappedMutexUnlock(&p->mutex);
}

void drawPlayer(player* p) {
	wrappedMutexLock(&screenMutex);
	wrappedMutexLock(&p->mutex);
	
	//clear and draw player
	consoleClearImage(p->prevRow, p->prevCol, SHIP_HEIGHT, SHIP_WIDTH);
	consoleDrawImage(p->row, p->col, SHIP[p->animTile], SHIP_HEIGHT);
	
	wrappedMutexUnlock(&screenMutex);
	//set the previous player position to where it was drawn
	//this is so the next time it is run, it will be cleared
	p->prevRow = p->row;
	p->prevCol = p->col;
	wrappedMutexUnlock(&p->mutex);
}

void movePlayer(player* p, int dRow, int dCol) {
	wrappedMutexLock(&p->mutex);
	int newRow = p->row + dRow;
	int newCol = p->col + dCol;
	wrappedMutexUnlock(&p->mutex);
	
	//check to see if new position is outside of player area
	//if so, return from function and do not move
	if(newRow + SHIP_HEIGHT > LOWER_GAME_BOUND) return;
	if(newRow < UPPER_PLAYER_BOUND) return;
	if(newCol + SHIP_WIDTH > RIGHT_GAME_BOUND) return;
	if(newCol < LEFT_GAME_BOUND) return;
	
	//else move to new position
	wrappedMutexLock(&p->mutex);
	p->row = newRow;
	p->col = newCol;
	wrappedMutexUnlock(&p->mutex);
}

void newPlayer(player* p) {	
	wrappedMutexLock(&p->mutex);
	p->col = p->startCol;
	p->row = p->startRow;
	p->animTile = 0;
	p->state = GAME;
	wrappedMutexUnlock(&p->mutex);
}

player* spawnPlayer(int startRow, int startCol, int lives) {
	player* p = malloc(sizeof(player));
	p->startRow = startRow;
	p->startCol = startCol;
	p->lives = lives;
	p->running = true;
	p->prevRow = startRow;
	p->prevCol = startCol;
	wrappedMutexInit(&(p->mutex), NULL);
	wrappedPthreadCreate(&(p->thread), NULL, runPlayer, (void*)p);
	return p;
}
