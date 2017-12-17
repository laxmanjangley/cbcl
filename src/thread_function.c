#include "thread_function.h"

Thread *init_thread(int oc, ConnectionPool *pool){
	Thread *t;
	t = malloc(sizeof(Thread));
	t->opcount = oc;
	t->pool = pool;
	return t;
}

void run(Init *param){
	unsigned int i;
	for(i=0;i<)
}