#include <stdlib.h>
#include "list.h"

bulletList* mallocBulletList() {
	bulletList* bl = malloc(sizeof(bulletNode)*2+sizeof(int));
	bl->length = 0;
	return bl;
}

void addBullet(bulletList* bl, bullet* b) {
	bulletNode* node = createBulletNode(b);
	
	if(bl->length == 0) {
		bl->head = node;
		bl->tail = node;
	}
	else {
		bl->tail->next = node;
		bl->tail = node;
	}
	bl->length++;
}

bulletNode* createBulletNode(bullet* b) {
	bulletNode* node = malloc(sizeof(node)+sizeof(bulletNode)+sizeof(bullet));
	node->payload = b;
	node->next = NULL;
	return node;
}
