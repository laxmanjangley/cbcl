#include "thread_func.h"
#include "thread_utils.h"

Thread *init_thread(int oc, ConnectionPool *pool){
	Thread *t;
	t = malloc(sizeof(Thread));
	t->opcount = oc;
	t->pool = create_pool();
	return t;
}

void run(Param *param){
	unsigned int i;
	//loop over all strings relevant to this file
	for(i=0; i<param->size; i++){
		const char* k = param->key[i];
		const char* v = param->value[i];
		Thread *t;
		t = param->thread;
		InstanceWrapper *iw, *iw_md5;
		iw = get_connection(t->pool, 3, 30);
		//get md5
		ConnStr md5cstr = {"couchbase://localhost:8091/md5bucket", "laxman", "123123"}
		iw_md5 = init(md5cstr);
		GetResult *gr;
		gr = get(k, iw_md5);
		if(gr){
			
		}
	}
}