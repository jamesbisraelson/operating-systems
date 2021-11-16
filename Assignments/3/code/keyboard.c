//keyboard.c
//holds all the functions to run the keyboard thread
#include <curses.h>
#include <sys/select.h>

#include "keyboard.h"
#include "player.h"
#include "centipede.h"
#include "threadwrappers.h"
#include "gameglobals.h"
#include "console.h"

void* runKeyboard(void* data) {
	player* p = (player*)data;
	
	while(true) {
		char c;
		//if gameover exit thread
		if(isGameOver()) {
			pthread_exit(NULL);
		}
		wrappedMutexLock(&screenMutex);
		//if keyboard hit, get the character hit and do something with it
		if(kbhit()) {
			c = getch();
			wrappedMutexLock(&p->mutex);
			if(p->state == GAME) {
				wrappedMutexUnlock(&p->mutex);
				//up left down right shoot quit
				switch(c) {
					case 'w':
						movePlayer(p, -1, 0);
						break;
					case 'a':
						movePlayer(p, 0, -1);
						break;
					case 's':
						movePlayer(p, 1, 0);
						break;
					case 'd':
						movePlayer(p, 0, 1);
						break;
					case ' ':
						shootBullet(p);
						wrappedMutexLock(&scoreMutex);
						score++;
						wrappedMutexUnlock(&scoreMutex);
						break;
					case 'q':
						putString(QUIT_STR, MSG_ROW, MSG_COL, QUIT_STR_LEN);
						wrappedMutexUnlock(&screenMutex);
						endGame();
						//if quit show message and exit thread
						pthread_exit(NULL);
				}
				//flush any exta input
				flushinp();
			}
			else {
				wrappedMutexUnlock(&p->mutex);
			}
		}		
		wrappedMutexUnlock(&screenMutex);
	}
}

int kbhit() {
	//check to see if there is any input available using select
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv) > 0;
}
