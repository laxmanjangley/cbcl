#ifndef INSTANCE_WRAPPER_H
#define INSTANCE_WRAPPER_H

#include <libcouchbase/couchbase.h>
#include <libcouchbase/api3.h>

typedef const char* ConnStr[3];

typedef struct Instance_t{
	lcb_t instance;
	unsigned int use_time;
	unsigned int last_use_time;
	unsigned int use_count;	
} InstanceWrapper;

typedef struct GetResult_t{
	const char* result;
	size_t nresult;
	lcb_error_t status;
} GetResult;

typedef struct {
	void (* const op_callback)(lcb_t instance, int cbtype, const lcb_RESPBASE *rb);
	InstanceWrapper* (* const init)(ConnStr cstr);
	void (* const begin)(InstanceWrapper *iw);
	void (* const end)(InstanceWrapper *iw);
	GetResult* (* const get)(const char *key, InstanceWrapper *iw);
	bool (*const set)(const char *key, const char *value, InstanceWrapper *iw);
} _Wrapper;

extern _Wrapper const Wrapper;

#endif
