#include "connection_pool.h"

void main(void){
	// ConnectionPool *pool;
	// pool = create_pool();
	Node *q = create();
	int a[9] = {1,2,3,4,5,6,7,8,9};
	int i=0;
	for(i;i<9;i++){
		push(q, (void *) a[i]);
	}
	for(i=0;i<4;i++){
		pop(q, 3);
	} 
}