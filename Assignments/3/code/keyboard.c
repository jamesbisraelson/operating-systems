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
		if(isGameOver()) {
			pthread_exit(NULL);
		}
		wrappedMutexLock(&screenMutex);
		if(kbhit()) {
			c = getch();
			wrappedMutexLock(&p->mutex);
			if(p->state == GAME) {
				wrappedMutexUnlock(&p->mutex);
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
						pthread_exit(NULL);
				}
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
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv) > 0;
}
