#include "instance_wrapper.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/md5.h>


char *md5_digest(const char *str, int length) {
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);
    MD5_Init(&c);
    while (length > 0) {
        if (length > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= 512;
        str += 512;
    }
	MD5_Final(digest, &c);
    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }
    return out;
}



void op_callback(lcb_t instance, int cbtype, const lcb_RESPBASE *rb)
{
	GetResult *gr = rb->cookie;
	gr->status = rb->rc;
    fprintf(stderr, "=== %s ===\n", lcb_strcbtype(cbtype));
    if (rb->rc == LCB_SUCCESS) {
        fprintf(stderr, "KEY: %.*s\n", (int)rb->nkey, rb->key);
        fprintf(stderr, "CAS: 0x%"PRIx64"\n", rb->cas);
        if (cbtype == LCB_CALLBACK_GET) {
            const lcb_RESPGET *rg = (const lcb_RESPGET *)rb;
            gr->result = rg->value;
            gr->nresult = rg->nvalue;
            fprintf(stderr, "VALUE: %.*s\n", (int)rg->nvalue, rg->value);
            fprintf(stderr, "FLAGS: 0x%x\n", rg->itmflags);
        }
    } else {
        fprintf(stderr, "Bucket operation failed: %s\n", lcb_strerror(NULL, err));
    }
    (void)instance;
}

InstanceWrapper *init(ConnStr cstr){
	InstanceWrapper *iw;
	iw = malloc(sizeof(InstanceWrapper));
	struct lcb_create_st create_options { 0 };
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
		fprintf(stderr, "Failed to connect libcouchbase instance: %s\n", lcb_strerror(NULL, err));
		return NULL;
	}
	lcb_wait(iw->instance);
    err = lcb_get_bootstrap_status(iw->instance);
    if (err != LCB_SUCCESS) {
        fprintf(stderr, "Failed to bootstrap libcouchbase instance: %s\n", lcb_strerror(NULL, err));
		return NULL;
    }
    lcb_install_callback3(iw->instance, LCB_CALLBACK_GET, op_callback);
    lcb_install_callback3(iw->instance, LCB_CALLBACK_STORE, op_callback);
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

GetResult *get(const char *key, InstanceWrapper *iw){
	lcb_CMDGET gcmd = { 0 };
	LCB_CMD_SET_KEY(&gcmd, key, strlen(key));
	lcb_error_t err;
	GetResult *cookie = (GetResult*) malloc(sizeof(GetResult));
	err = lcb_get3(iw->instance, (void*) cookie, &gcmd);
	if(err != LCB_SUCCESS){
		fprintf(stderr, "key not found: %s\n", lcb_strerror(NULL, err));
		return NULL;
	}
	lcb_wait(iw->instance);
	return cookie;
}

bool set(const char *key, const char *value, InstanceWrapper *iw){
	lcb_CMDSTORE scmd = { 0 };
	LCB_CMD_SET_KEY(&scmd, key, strlen(key));
	LCB_CMD_SET_VALUE(&scmd, value, strlen(value));
	lcb_error_t err;
	err = lcb_store3(iw->instance, NULL, &scmd);
	if(error != LCB_SUCCESS) return false;
	lcb_wait(iw->instance);
}

_Wrapper const Wrapper = {
	op_callback,
	init,
	begin,
	end,
	get,
	set
};
