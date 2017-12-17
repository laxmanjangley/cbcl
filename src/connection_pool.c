#include "connection_pool.h"


InstanceWrapper *_make_connection(ConnectionPool *pool){
	InstanceWrapper *iw;
	iw = init(pool->cstr);
	pool->curr_nodes[pool->current_connections] = iw;
	pool->current_connections += 1;
	return iw;
}

ConnectionPool *create_pool(){
	ConnectionPool *pool;
	pool = malloc(sizeof(ConnectionPool));
	// pool->handle = malloc(sizeof(Node));
	pool->handle = create();
	pool->current_connections = 0;
	int i;
	for(i=0;i<INITIAL_CONNECTIONS;i++){
		InstanceWrapper *iw;
		iw = _make_connection(pool);
		push(pool->handle, (void*) iw);
	}
}

InstanceWrapper *get_connection(ConnectionPool *pool, unsigned int initial_wait, unsigned int next_wait){
	InstanceWrapper *iw = (InstanceWrapper*) pop(pool->handle, initial_wait);
	if(iw == NULL){
		if(pool->current_connections == MAX_CONNECTIONS){
			fprintf(stderr, "Cannot create a new connection, pool is full!!");
			return NULL;
		} else {
			iw = _make_connection(pool);
		}
		if(iw) {
			pool->current_connections += 1;
			begin(iw);
			return iw;
		} else {
			iw = (InstanceWrapper*) pop(pool->handle, next_wait);
			if(iw) return iw;
			return NULL;
		}
	} else {
		return iw;
	}
}

void release_connection(ConnectionPool *pool, InstanceWrapper *iw){
	end(iw);
	push(pool->handle, (void*) iw);
}

_Pool const Pool = {
	_make_connection,
	create_pool,
	get_connection,
	release_connection
};