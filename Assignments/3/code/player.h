#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <pthread.h>

#define SHIP_HEIGHT 3
#define SHIP_WIDTH 3
#define SHIP_ANIM_TILES 2

//the ship animation
extern char* SHIP[SHIP_ANIM_TILES][SHIP_HEIGHT];

//the three states the player can be in
typedef enum player_state {
	GAME,
	DEAD,
	GAMEOVER
} playerState;

//all of the information for the player is held here
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

//takes a player and shoots a bullet from its position
void shootBullet(player* p);

//takes a player and increments its animation
void nextPlayerAnim(player* p);

//takes a player and draws it to the buffer
void drawPlayer(player* p);

//the function passed to the thread for operating the player
void* runPlayer(void* data);

//sets a new player at the original starting position
void newPlayer(player* p);

//mallocs a player struct and starts the player thread
player* spawnPlayer(int startCol, int startRow, int lives);

//moves the player by a difference of dRow and dCol
void movePlayer(player* p, int dRow, int dCol);

#endif
