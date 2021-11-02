#include <stdlib.h>
#include "player.h"
#include "threadwrappers.h"
#include "console.h"
#include "gameglobals.h"
#include "centipede.h"

#define START_COL 20
#define START_ROW 38

char* SHIP[SHIP_ANIM_TILES][SHIP_HEIGHT] = {
{
	" /\\ ",
	" || ",
	"/--\\",
	" (  ",
	"  ) "
},
{
	" /\\ ",
	" || ",
	"/--\\",
	"  ) ",
	" (  "
}};

void* runPlayer(void* data) {
	player* p = (player*)data;
	newPlayer(p);

	while(true) {
		wrappedMutexLock(&screenMutex);
		wrappedMutexLock(&(p->mutex));
		consoleClearImage(p->row, p->col-1, SHIP_HEIGHT, SHIP_WIDTH);
		consoleDrawImage(p->row, p->col, SHIP[p->animTile], SHIP_HEIGHT);
		wrappedMutexUnlock(&screenMutex);
		p->animTile++;
		p->animTile %= SHIP_ANIM_TILES;
		wrappedMutexUnlock(&(p->mutex));
		
		wrappedMutexLock(&gameOverMutex);
		if(gameOver) {
			wrappedMutexUnlock(&gameOverMutex);
			free(p);
			pthread_exit(NULL);
		}
		wrappedMutexUnlock(&gameOverMutex);
		sleepTicks(10);		
	}
	return NULL;
}

void movePlayer(player* p, int dRow, int dCol) {
	p->row += dRow;
	p->col += dCol;
}

void newPlayer(player* p) {
	
	p->col = p->startCol;
	p->row = p->startRow;
	p->animTile = 0;
	p->state = GAME;
}

player* spawnPlayer(int startRow, int startCol, int lives) {
	player* p = malloc(sizeof(player));
	p->startRow = startRow;
	p->startCol = startCol;
	p->lives = lives;
	p->running = true;
	
	wrappedMutexInit(&(p->mutex), NULL);
	wrappedPthreadCreate(&(p->thread), NULL, runPlayer, (void*)p);
	return p;
}
