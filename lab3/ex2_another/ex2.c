/*************************************
 * Lab 3 Exercise 2
 * Name: Lin Yuyang
 * Student No: A0207526H
 * Lab Group: 09
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_cond_init(&(lock->cond), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
  lock->writer_wait_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  lock->writer_wait_count++;
  while(lock->reader_count > 0 || lock->writer_count > 0){
    pthread_cond_wait(&(lock->cond), &(lock->mutex));
  }
  lock->writer_count++;
  pthread_mutex_unlock(&(lock->mutex));
}

void writer_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  lock->writer_count--;
  lock->writer_wait_count--;
  pthread_cond_broadcast(&(lock->cond));
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  while(lock->writer_wait_count>0){
    pthread_cond_wait(&(lock->cond), &(lock->mutex));
  }
  while(lock->writer_count>0){
    pthread_cond_wait(&(lock->cond), &(lock->mutex));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count--;
  pthread_cond_broadcast(&(lock->cond));
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->mutex));
  pthread_cond_destroy(&(lock->cond));
}
