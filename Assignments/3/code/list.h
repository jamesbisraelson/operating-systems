#ifndef LIST_H
#define LIST_H

#include "bullet.h"

typedef struct bullet_node {
	bullet* payload;
	struct bullet_node* next;
} bulletNode;

typedef struct bullet_list {
	bulletNode* head;
	bulletNode* tail;
	int length;
} bulletList;

bulletNode* createBulletNode(bullet* b);
bulletList* mallocBulletList();
void addBullet(bulletList* bl, bullet* b);

#endif
