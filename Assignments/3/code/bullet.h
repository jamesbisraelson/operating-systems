#include <pthread.h>
#include <stdbool.h>

typedef enum bullet_type {
	PLAYER,
	ENEMY
} bulletType;

typedef struct bullet_struct {
	int row;
	int col;
	int prevRow;
	int velocity;
	bool isAlive;
	bulletType type;
	pthread_t thread;
	pthread_mutex_t mutex;
} bullet;

bullet* spawnBullet(int startRow, int startCol, bulletType type);
void drawBullet(bullet* b);
void moveBullet(bullet* b);
void newBullet(bullet* b);
void* runBullet(void* data);
