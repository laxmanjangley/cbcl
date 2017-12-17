#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#define MAX_CONNECTIONS 10
#define INITIAL_CONNECTIONS 4

#include "queue.h"
#include "instance_wrapper.h"

typedef struct ConnectionPool_t{
	Node *handle;
	Node *curr_nodes[MAX_CONNECTIONS];
	int current_connections;
	ConnStr cstr;
} ConnectionPool;

typedef struct {
	InstanceWrapper* (* const _make_connection)(ConnectionPool *pool);
	ConnectionPool* (* const create_pool)();
	InstanceWrapper* (* const get_connection)(ConnectionPool *pool);
	void (* const release_connection)(ConnectionPool *pool, InstanceWrapper *iw);
} _Pool;

extern _Pool const Pool;

#endif