#ifndef GAMEGLOBALS_H
#define GAMEGLOBALS_H

#include <stdbool.h>

//upper and lower bounds for bullets/enemies
#define LOWER_GAME_BOUND 28
#define UPPER_GAME_BOUND 2

//upper bound for player
#define UPPER_PLAYER_BOUND 17

//left and right bounds of game
#define RIGHT_GAME_BOUND 80
#define LEFT_GAME_BOUND 0

#define LEFT -1
#define RIGHT 1
#define UP -1
#define DOWN 1

extern bool gameOver;
extern char* GAME_BOARD[];

#endif
