#ifndef THREAD_FUNC_H
#define THREAD_FUNC_H

#include "connection_pool.h"

typedef struct Thread_t{
	ConnectionPool *pool;
	unsigned int opcount;
} Thread;

typedef struct Param_t{
	const char** key;
	const char** value;
	unsigned int size;
	Thread *thread;
} Param;

typedef struct {
	Thread* (*void init_thread)(int oc, ConnectionPool *pool);
	void (*void run)(Param *param);
} _Func;

extern _Func const Func;

#endif