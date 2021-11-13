#include "gameglobals.h"

#include "console.h"
#include "threadwrappers.h"


pthread_mutex_t screenMutex;
pthread_mutex_t gameOverMutex;
pthread_mutex_t randomMutex;
pthread_mutex_t bulletListMutex;
pthread_mutex_t enemyListMutex;

pthread_cond_t gameOverCond;

pthread_t screenThread;
pthread_t keyboardThread;
pthread_t spawnerThread;
bulletList* bList;
enemyList* eList;

bool gameOver = false;

char *GAME_BOARD[] = {
"Score:          Lives:",
"=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-centipiede!=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"................................................................................",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
""};
