#include <stdlib.h>
#include "list.h"
#include "threadwrappers.h"
#include "gameglobals.h"

bulletList* mallocBulletList() {
	bulletList* bl = malloc(sizeof(bulletList));
	bl->length = 0;
	return bl;
}

void addBullet(bulletList* bl, bullet* b) {
	bulletNode* node = createBulletNode(b);
	
	wrappedMutexLock(&bulletListMutex);	
	if(bl->length == 0) {
		bl->head = node;
		bl->tail = node;
	}
	else {
		bl->tail->next = node;
		bl->tail = node;
	}
	bl->length++;
	wrappedMutexUnlock(&bulletListMutex);	
}

bulletNode* createBulletNode(bullet* b) {
	bulletNode* node = malloc(sizeof(bulletNode));
	node->payload = b;
	node->next = NULL;
	return node;
}

void freeAndJoinBulletList(bulletList* bl) {
	wrappedMutexLock(&bulletListMutex);	
	bulletNode* node = bl->head;
	int i;
	for(i=0; i<bl->length; i++) {
		wrappedPthreadJoin(node->payload->thread, NULL);
		free(node->payload);
		
		bulletNode* toFree = node;
		node = node->next;

		toFree->next = NULL;
		free(toFree);
	}
	wrappedMutexUnlock(&bulletListMutex);	
}

enemyList* mallocEnemyList() {
	enemyList* el = malloc(sizeof(enemyList));
	el->length = 0;
	return el;
}

void addEnemy(enemyList* el, enemy* e) {
	enemyNode* node = createEnemyNode(e);
	
	wrappedMutexLock(&enemyListMutex);	
	if(el->length == 0) {
		el->head = node;
		el->tail = node;
	}
	else {
		el->tail->next = node;
		el->tail = node;
	}
	el->length++;
	wrappedMutexUnlock(&enemyListMutex);	
}

enemyNode* createEnemyNode(enemy* e) {
	enemyNode* node = malloc(sizeof(enemyNode));
	node->payload = e;
	node->next = NULL;
	return node;
}

static void freeAndJoinEnemy(enemy* e) {
	wrappedMutexLock(&e->mutex);
	segment* s = e->head;
	int i;
	for(i=0; i<e->length; i++) {
		segment* toFree = s;
		s = s->next;
		free(toFree);
	}
	wrappedMutexUnlock(&e->mutex);
	wrappedPthreadJoin(e->thread, NULL);
	free(e);
}

void freeAndJoinEnemyList(enemyList* el) {
	wrappedMutexLock(&enemyListMutex);	
	enemyNode* node = el->head;
	int i;
	for(i=0; i<el->length; i++) {
		freeAndJoinEnemy(node->payload);
		
		enemyNode* toFree = node;
		node = node->next;

		toFree->next = NULL;
		free(toFree);
	}
	wrappedMutexUnlock(&enemyListMutex);	
}
