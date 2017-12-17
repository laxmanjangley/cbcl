#ifndef INSTANCE_WRAPPER_H
#define INSTANCE_WRAPPER_H

#include <libcouchbase/couchbase.h>

typedef const char* ConnStr[3];

typedef struct Instance_t{
	lcb_t instance;
	unsigned int use_time;
	unsigned int last_use_time;
	unsigned int use_count;	
} InstanceWrapper;

typedef struct {
	InstanceWrapper* (* const init)(ConnStr cstr);
	void (* const begin)(InstanceWrapper *iw);
	void (* const end)(InstanceWrapper *iw);
} _Wrapper;


extern _Wrapper const Wrapper;

#endif
