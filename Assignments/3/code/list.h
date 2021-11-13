#ifndef LIST_H
#define LIST_H

#include "bullet.h"
#include "enemy.h"

typedef struct bullet_node {
	bullet* payload;
	struct bullet_node* next;
} bulletNode;

typedef struct bullet_list {
	bulletNode* head;
	bulletNode* tail;
	int length;
} bulletList;

typedef struct enemy_node {
	enemy* payload;
	struct enemy_node* next;
} enemyNode;

typedef struct enemy_list {
	enemyNode* head;
	enemyNode* tail;
	int length;
} enemyList;

bulletNode* createBulletNode(bullet* b);
bulletList* mallocBulletList();
void addBullet(bulletList* bl, bullet* b);
void freeAndJoinBulletList(bulletList* bl);

enemyNode* createEnemyNode(enemy* e);
enemyList* mallocEnemyList();
void addEnemy(enemyList* el, enemy* e);
void freeAndJoinEnemyList(enemyList* el);

#endif
