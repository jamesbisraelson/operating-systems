#ifndef PLAYER_H
#define PLAYER_H

#define SHIP_HEIGHT 5
#define SHIP_WIDTH 4
#define SHIP_ANIM_TILES 2

#include "threadwrappers.h"
#include "console.h"
#include "gameglobals.h"
#include "centipede.h"

extern const char* SHIP[SHIP_ANIM_TILES][SHIP_HEIGHT];

struct player_struct {
	int startCol;
	int startRow;
	//playerState state;
	//bool running;
	//int lives;
	//int row;
	//int col;
	//int animTile;
	//pthread_t thread;
	//pthread_mutex_t mutex;
} player;

void* runPlayer(void* dummy);

#endif
