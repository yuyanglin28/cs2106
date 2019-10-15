/*************************************
 * Lab 3 Exercise 2
 * Name: Lin Yuyang
 * Student No: A0207526H
 * Lab Group: 09
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t mutex;
  int reader_count;
  int writer_count;
  int writer_wait_count;
  pthread_cond_t cond;
} rw_lock;
