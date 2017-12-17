all : main

main : main.o connection_pool.o instance_wrapper.o queue.o
	gcc -g -o $@ $^ -lpthread -lcouchbase

%.o : %.c
	gcc -g -o $@ -c $< -lpthread -lcouchbase

main.o : connection_pool.h instance_wrapper.h queue.h
connection_pool.o : connection_pool.h instance_wrapper.h queue.h
instance_wrapper.o : instance_wrapper.h
queue.o : queue.h

clean : 
	rm -rf *.o *.gch main