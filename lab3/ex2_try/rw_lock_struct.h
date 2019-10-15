/*************************************
 * Lab 3 Exercise 2
 * Name: Lin Yuyang
 * Student No: A0207526H
 * Lab Group: 09
 *************************************/


#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_mutex_t empty;
  pthread_mutex_t mutex3;
  int reader_count;
  int writer_count;

  int writer_wait_count;
} rw_lock;
