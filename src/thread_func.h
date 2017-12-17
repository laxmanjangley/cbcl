#ifndef THREAD_FUNC_H
#define THREAD_FUNC_H

#include "connection_pool.h"

typedef struct Thread_t{
	ConnectionPool *pool;
	unsigned int opcount;
} Thread;

typedef struct Init_t{
	const char** input;
	unsigned int size;
} Init;

typedef struct {
	Thread* (*void init_thread)(int oc, ConnectionPool *pool);
	void (*void run)(Init *param);
} _Func;

extern _Func const Func;

#endif