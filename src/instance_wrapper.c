#include "instance_wrapper.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// #include <libcouchbase/couchbase.h>

// ConnStr conn_str
InstanceWrapper *init(ConnStr cstr){
	InstanceWrapper *iw;
	iw = malloc(sizeof(InstanceWrapper));
	struct lcb_create_st create_options;
	lcb_error_t err;
	memset(&create_options, 0, sizeof(create_options));
	create_options.version = 3;
	create_options.v.v3.connstr = cstr[0];
	create_options.v.v3.username = cstr[1];
	create_options.v.v3.passwd = cstr[2];
	if((err = lcb_create(&(iw->instance), &create_options)) != LCB_SUCCESS){
		fprintf(stderr, "Failed to create libcouchbase instance: %s\n", lcb_strerror(NULL, err));
		return NULL;
	}
	if((err = lcb_connect(iw->instance)) != LCB_SUCCESS){
		fprintf(stderr, "Failed to create libcouchbase instance: %s\n", lcb_strerror(NULL, err));
		return NULL;
	}
	iw->last_use_time = 0;
	iw->use_count = 0;
	iw->use_time = 0;
	return iw;
}

void begin(InstanceWrapper *iw){
	iw->last_use_time = clock(); 
}

void end(InstanceWrapper *iw){
	iw->use_time += clock() - iw->last_use_time;
	iw->use_count += 1;
}

_Wrapper const Wrapper = {
	init,
	begin,
	end
};
// void die(lcb_t instance, const char *msg, lcb_error_t err){
//     fprintf(stderr, "%s. Received code 0x%X (%s)\n",
//         msg, err, lcb_strerror(instance, err));
//     exit(0);
// }