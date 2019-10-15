/*************************************
 * Lab 3 Exercise 1
 * Name: Lin Yuyang
 * Student No: A0207526H
 * Lab Group: 09
 *************************************/
 #include <pthread.h>
 #include <semaphore.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include "rw_lock.h"

 void initialise(rw_lock* lock)
 {
   pthread_mutex_init(&(lock->mutex), NULL);
   pthread_mutex_init(&(lock->empty), NULL);
   pthread_mutex_init(&(lock->mutex3), NULL);
   lock->reader_count = 0;
   lock->writer_count = 0;
   lock->writer_wait_count = 0;
 }

 void writer_acquire(rw_lock* lock)
 {
   pthread_mutex_lock(&(lock->mutex3));
   lock->writer_wait_count ++;
   //pthread_mutex_lock(&(lock->empty));
     pthread_mutex_unlock(&(lock->mutex3));

     pthread_mutex_lock(&(lock->empty));

   lock->writer_count++;
   //pthread_mutex_unlock(&(lock->mutex3));
 }

 void writer_release(rw_lock* lock)
 {
   lock->writer_count--;
   lock->writer_wait_count --;
   pthread_mutex_unlock(&(lock->empty));
 }

 void reader_acquire(rw_lock* lock)
 {
   pthread_mutex_lock(&(lock->mutex));
   if (lock->writer_wait_count != 0){
     pthread_mutex_lock()
   }
   if (lock->reader_count == 0)
     pthread_mutex_lock(&(lock->empty));
   lock->reader_count++;
   pthread_mutex_unlock(&(lock->mutex));
 }

 void reader_release(rw_lock* lock)
 {
   pthread_mutex_lock(&(lock->mutex));
   lock->reader_count--;
   if (lock->reader_count == 0)
     pthread_mutex_unlock(&(lock->empty));

   pthread_mutex_unlock(&(lock->mutex));
 }

 void cleanup(rw_lock* lock)
 {
   pthread_mutex_destroy(&(lock->mutex));
   pthread_mutex_destroy(&(lock->empty));
 }

 /*void barrier (int n){
   sem_wait(&mutex2);
   arrived ++;
   sem_post(&mutex2);

   if (arrived != n){
     sem_wait(&wait);
   }else {
     for (int i=0; i<n-1; i++)
       sem_post(&wait);
   }

 }*/
