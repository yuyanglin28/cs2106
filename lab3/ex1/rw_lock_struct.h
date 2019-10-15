/*************************************
 * Lab 3 Exercise 1
 * Name: Lin Yuyang
 * Student No: A0207526H
 * Lab Group: 09
 *************************************/

 #include <pthread.h>

 typedef struct {
   pthread_mutex_t mutex;
   pthread_mutex_t empty;
   int reader_count;
   int writer_count;
 } rw_lock;
