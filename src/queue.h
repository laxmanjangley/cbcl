#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct Data_t {
	void *prev;
	void *value;
} Data;

typedef struct Node_t {
	Data *head;
	Data *tail;
	pthread_mutex_t *mutex;
} Node;


typedef struct {
	Node* (* const create)();
	void (* const destroy)(Node *handle);
	void (* const push)(Node *handle, void *elem);
	void* (* const pop)(Node *handle, unsigned int wait);
} _Queue;

extern _Queue const Queue;

#endif
