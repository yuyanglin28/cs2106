/*************************************
 * Lab 3 Exercise 1
 * Name: Lin Yuyang
 * Student No: A0207526H
 * Lab Group: 09
 *************************************/
 #include <pthread.h>

 #include "rw_lock.h"

 void initialise(rw_lock* lock)
 {
   pthread_mutex_init(&(lock->mutex), NULL);
   pthread_mutex_init(&(lock->empty), NULL);
   pthread_mutex_init(&(lock->door), NULL);
   lock->reader_count = 0;
   lock->writer_count = 0;
 }

 void writer_acquire(rw_lock* lock)
 {
   pthread_mutex_lock(&(lock->door));

   pthread_mutex_lock(&(lock->empty));

   lock->writer_count++;
 }

 void writer_release(rw_lock* lock)
 {
   lock->writer_count--;
   pthread_mutex_unlock(&(lock->empty));
   pthread_mutex_unlock(&(lock->door));
 }

 void reader_acquire(rw_lock* lock)
 {
   pthread_mutex_lock(&(lock->door));
   pthread_mutex_unlock(&(lock->door));
   pthread_mutex_lock(&(lock->mutex));
   if (lock->reader_count == 0)
     pthread_mutex_lock(&(lock->empty));
   lock->reader_count++;
   pthread_mutex_unlock(&(lock->mutex));
 }

 void reader_release(rw_lock* lock)
 {
   pthread_mutex_lock(&(lock->mutex));
   lock->reader_count--;
   if (lock->reader_count == 0){
     pthread_mutex_unlock(&(lock->empty));
   }
   pthread_mutex_unlock(&(lock->mutex));
 }

 void cleanup(rw_lock* lock)
 {
   pthread_mutex_destroy(&(lock->mutex));
   pthread_mutex_destroy(&(lock->empty));
   pthread_mutex_destroy(&(lock->door));
 }
