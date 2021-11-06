#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <pthread.h>

#define SHIP_HEIGHT 5
#define SHIP_WIDTH 5
#define SHIP_ANIM_TILES 2


extern char* SHIP[SHIP_ANIM_TILES][SHIP_HEIGHT];

typedef enum player_state {
	GAME,
	DEAD,
	GAMEOVER
} playerState;

typedef struct player_struct {
	int startCol;
	int startRow;
	int col;
	int row;
	int prevCol;
	int prevRow;
	bool running;
	int lives;
	int animTile;
	playerState state;
	pthread_t thread;
	pthread_mutex_t mutex;
} player;

void shootBullet(player* p);
void nextAnim(player* p);
void drawPlayer(player* p);
void* runPlayer(void* data);
void newPlayer(player* p);
player* spawnPlayer(int startCol, int startRow, int lives);
void movePlayer(player* p, int dRow, int dCol);

#endif
