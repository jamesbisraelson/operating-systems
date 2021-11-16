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

void freeBulletList(bulletList* bl) {
	wrappedMutexLock(&bulletListMutex);	
	bulletNode* node = bl->head;
	int i;
	for(i=0; i<bl->length; i++) {
		free(node->payload);
		
		bulletNode* toFree = node;
		node = node->next;
		free(toFree);
	}
	wrappedMutexUnlock(&bulletListMutex);
}

void joinBulletList(bulletList* bl) {
	wrappedMutexLock(&bulletListMutex);	
	bulletNode* node = bl->head;
	int i;
	for(i=0; i<bl->length; i++) {
		wrappedMutexLock(&node->payload->mutex);
		if(!node->payload->isAlive && !node->payload->isJoined) {
			node->payload->isJoined = true;
			wrappedPthreadJoin(node->payload->thread, NULL);
			wrappedMutexUnlock(&node->payload->mutex);
		}
		else wrappedMutexUnlock(&node->payload->mutex);	
		node = node->next;
	}
	wrappedMutexUnlock(&bulletListMutex);	
}

enemyList* mallocEnemyList() {
	enemyList* el = malloc(sizeof(enemyList));
	el->length = 0;
	el->liveEnemies = 0;
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
	el->liveEnemies++;
	wrappedMutexUnlock(&enemyListMutex);	
}

enemyNode* createEnemyNode(enemy* e) {
	enemyNode* node = malloc(sizeof(enemyNode));
	node->payload = e;
	node->next = NULL;
	return node;
}

static void freeEnemy(enemy* e) {
	segment* s = e->head;
	int i;
	for(i=0; i<e->length; i++) {
		segment* toFree = s;
		s = s->next;
		free(toFree);
	}
	free(e);
}

void freeEnemyList(enemyList* el) {
	wrappedMutexLock(&enemyListMutex);	
	enemyNode* node = el->head;
	int i;
	for(i=0; i<el->length; i++) {
		freeEnemy(node->payload);
		
		enemyNode* toFree = node;
		node = node->next;
		free(toFree);
	}
	wrappedMutexUnlock(&enemyListMutex);
}

void joinEnemyList(enemyList* el) {
	wrappedMutexLock(&enemyListMutex);	
	enemyNode* node = el->head;
	int i;
	for(i=0; i<el->length; i++) {
		wrappedMutexLock(&node->payload->mutex);
		if(!node->payload->isAlive && !node->payload->isJoined) {
			node->payload->isJoined = true;
			el->liveEnemies--;
			wrappedPthreadJoin(node->payload->thread, NULL);
			wrappedMutexUnlock(&node->payload->mutex);
		}
		else wrappedMutexUnlock(&node->payload->mutex);	
		node = node->next;
	}
	wrappedMutexUnlock(&enemyListMutex);	
}
