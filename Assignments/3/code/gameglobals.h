//gameglobals.h
//the .h file for gameglobals.c

#ifndef GAMEGLOBALS_H
#define GAMEGLOBALS_H

#include <stdbool.h>
#include "list.h"

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

//game over messages
#define GAME_OVER_STR "you suck."
#define QUIT_STR "quitter..."
#define QUIT_STR_LEN 11
#define MSG_ROW 9
#define MSG_COL 30

//how  long to sleep when player dies
#define DEAD_SLEEP 20

//global mutexes
extern pthread_mutex_t screenMutex;
extern pthread_mutex_t gameOverMutex;
extern pthread_mutex_t randomMutex;
extern pthread_mutex_t bulletListMutex;
extern pthread_mutex_t enemyListMutex;
extern pthread_mutex_t scoreMutex;

//game over condition
extern pthread_cond_t gameOverCond;

//global threads
extern pthread_t keyboardThread;
extern pthread_t screenThread;
extern pthread_t spawnerThread;
extern pthread_t cleanupThread;

//global bullet and enemy lists
extern bulletList* bList;
extern enemyList* eList;

//the global player struct
extern player* p;

//other global variables
extern int score;
extern bool gameOver;
extern char* GAME_BOARD[];

//checks if the game is over
bool isGameOver();

//sets the game to done and signals the gameOverCond
void endGame();
#endif
