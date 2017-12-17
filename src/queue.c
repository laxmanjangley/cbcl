#include "queue.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

Node* create() {
  Node *handle = malloc(sizeof(*handle));
  handle->head = NULL;
  handle->tail = NULL;
  pthread_mutex_t *mutex = malloc(sizeof(*mutex));
  handle->mutex = mutex;  
  return handle;
}

void destroy(Node *node) {
  free(node->mutex);
  free(node);
  node = NULL;
}

void push(Node *node, void *elem, unsigned int wait) {
  Data *data = malloc(sizeof(Data));
  data->value = elem;
  data->prev = NULL;
  pthread_mutex_lock(node->mutex);
  if (node->head == NULL) {
    node->head = data;
    node->tail = data;
  } else {
    Data* old_tail = node->tail;
    old_tail->prev = data;
    node->tail = data;
  }
  pthread_mutex_unlock(node->mutex);
}

void* pop(Node *node, unsigned int wait) {

  struct timespec *timeout;
  int ret;
  timeout = (struct timespec*) malloc(sizeof(struct timespec));
  timeout->tv_sec = wait;
  timeout->tv_nsec = 0;
  ret =  pthread_mutex_timedlock(node->mutex, timeout);
  // pthread_mutex_lock(node->mutex);
  if(!ret){
    Data *head = node->head;
    if (head == NULL) {
      pthread_mutex_unlock(node->mutex);
      return NULL;
    } else {
      node->head = head->prev;
      void *value = head->value;
      free(head);    
      pthread_mutex_unlock(node->mutex);
      fprintf(stdout, "Got %d\n", (unsigned int*) value);
      return value;
    }
  } else {
    fprintf(stderr, "Locked for pop timed out");
    return NULL;
  }
}

_Queue const Queue = {
  create,
  destroy,
  push,
  pop
};
