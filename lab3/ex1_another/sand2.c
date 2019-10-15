/*************************************
 * Lab 3 Exercise 1
 *************************************
You may change this file during your own testing,
but note that they will be replaced with the original
files when we test your assignments.
 *************************************/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  pthread_mutex_t mutex;
  int reader_count;
  int writer_count, writer_wait_count;
  pthread_cond_t reader_gate;
  pthread_cond_t writer_gate;
} rw_lock;

int READERS;
int WRITERS;
int WRITE_COUNT;
int READ_COUNT;

int value = 0;
int max_concurrent_readers = 0;
pthread_mutex_t max_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t reader_gate = PTHREAD_COND_INITIALIZER;
//pthread_cond_t writer_gate = PTHREAD_COND_INITIALIZER;

rw_lock* read_write_lock;

void* writer(void* threadid);
void* reader(void* threadid);

int main(int argc, char** argv)
{
    if (argc < 5) {
        printf("Usage: %s readers writers read_count write_count\n", argv[0]);
        exit(1);
    }

    READERS = atoi(argv[1]);
    WRITERS = atoi(argv[2]);
    READ_COUNT = atoi(argv[3]);
    WRITE_COUNT = atoi(argv[4]);

    pthread_t writer_threads[WRITERS];
    pthread_t reader_threads[READERS];

    read_write_lock = malloc(sizeof(rw_lock));
    if (read_write_lock == NULL) {
        printf("Lock failed to be allocated.\n");
        exit(1);
    }

    //initialise(read_write_lock);
    pthread_mutex_init(&(read_write_lock->mutex), NULL);
    pthread_cond_init(&(read_write_lock->reader_gate), NULL);
    pthread_cond_init(&(read_write_lock->writer_gate), NULL);
    //read_write_lock->reader_gate = PTHREAD_COND_INITIALIZER;
    //read_write_lock->writer_gate = PTHREAD_COND_INITIALIZER;
    read_write_lock->reader_count = 0;
    read_write_lock->writer_count = 0;

    int thread_id;
    int i, return_code;
    int bad_threads = 0;
    void* thread_return;
    int *pi;

    for (i = 0; i < READERS; i++) {
        thread_id = i + 1;
        pi = malloc(sizeof(int));
        if (pi == NULL) {
            printf("Pointer allocation failed.\n");
            exit(1);
        }
        *pi = thread_id;
#ifdef DEBUG
        printf("Creating thread for reader #%ld\n", thread_id);
#endif
        return_code = pthread_create(&reader_threads[i], NULL, reader, (void*)pi);
        if (return_code) {
            printf("Error while creating thread. Return code: %d\n", return_code);
            exit(1);
        }
    }

    for (i = 0; i < WRITERS; i++) {
        thread_id = i + 1;
        pi = malloc(sizeof(int));
        if (pi == NULL) {
            printf("Pointer allocation failed.\n");
            exit(1);
        }
        *pi = thread_id;
#ifdef DEBUG
        printf("Creating thread for writer #%ld\n", thread_id);
#endif
        return_code = pthread_create(&writer_threads[i], NULL, writer, (void*)pi);
        if (return_code) {
            printf("Error while creating thread. Return code: %d\n", return_code);
            exit(1);
        }

    }



    for (i = 0; i < WRITERS; i++) {
        pthread_join(writer_threads[i], &thread_return);
        bad_threads += *(int*)thread_return;
        free (thread_return);
    }

    for (i = 0; i < READERS; i++) {
        pthread_join(reader_threads[i], &thread_return);
        bad_threads += *(int*)thread_return;
        free (thread_return);

    }

    pthread_mutex_destroy(&max_mutex);

    //cleanup(read_write_lock);
    pthread_mutex_destroy(&(read_write_lock->mutex));
    pthread_cond_destroy(&(read_write_lock->reader_gate));
    pthread_cond_destroy(&(read_write_lock->writer_gate));

    free(read_write_lock);

    if (bad_threads) {
        printf("Program failed: %d bad threads found.\n", bad_threads);
        return -1;
    }
    printf("SUCCESS!\n");
    printf("Total writes: %d, Total reads: %d, Max Concurrent Readers: %d\n",
           WRITERS * WRITE_COUNT,
           READERS * READ_COUNT,
           max_concurrent_readers);
    return 0;
}

// Each writer thread runs this function.
// It writes its `threadid` to `value` `WRITE_COUNT` times.
void* writer(void* threadid)
{
    int i;
    int error_found = 0;
    int tid = *(int*)threadid;
    int *pi;
    free (threadid);

    for (i = 0; i < WRITE_COUNT; i++) {

      /*pthread_mutex_lock(&(read_write_lock->mutex));
      read_write_lock->writer_count++;
      pthread_mutex_unlock(&(read_write_lock->mutex));*/

      pthread_mutex_lock(&(read_write_lock->mutex));
      read_write_lock->writer_wait_count++;
      while(read_write_lock->reader_count > 0 || read_write_lock->writer_count > 0){
        pthread_cond_wait(&(read_write_lock->writer_gate), &(read_write_lock->mutex));
      }
      read_write_lock->writer_wait_count--;
      read_write_lock->writer_count++;
      pthread_mutex_unlock(&(read_write_lock->mutex));

        if (read_write_lock->reader_count != 0 || read_write_lock->writer_count != 1) {
            printf("Writer %d: Reader or another writer found while attempting to write\n",
                    tid);
            error_found = 1;
        }
#ifdef DEBUG
        printf("[Writer: #%d, Loop: %d] ", tid, i);
#endif
        printf("Writer %d writes.\n", tid);
        value = tid;

        /*pthread_mutex_lock(&(read_write_lock->mutex));
        read_write_lock->writer_count--;
        pthread_mutex_unlock(&(read_write_lock->mutex));*/

        //writer_release(read_write_lock);
        pthread_mutex_lock(&(read_write_lock->mutex));
        read_write_lock->writer_count--;
        if(read_write_lock->writer_wait_count > 0){
          pthread_cond_signal(&(read_write_lock->writer_gate));
        }
        pthread_cond_broadcast(&(read_write_lock->reader_gate));
        pthread_mutex_unlock(&(read_write_lock->mutex));
    }
    pi = malloc(sizeof(int));
    if (pi == NULL) {
        printf("Pointer allocation failed.\n");
        exit(1);
    }
    *pi = error_found;
    pthread_exit((void*)pi);
}

// Each reader thread runs this function.
// It reads and `value` `READ_COUNT` times, and finds the number of
// concurrent readers.
void* reader(void* threadid)
{
    int i, curr_readers;
    int error_found = 0;
    int tid = *(int*)threadid;
    int *pi;
    free (threadid);

    for (i = 0; i < READ_COUNT; i++) {

      /*pthread_mutex_lock(&(read_write_lock->mutex));
      read_write_lock->reader_count++;
      pthread_mutex_unlock(&(read_write_lock->mutex));*/

        //reader_acquire(read_write_lock);
        pthread_mutex_lock(&(read_write_lock->mutex));
        while(read_write_lock->writer_count>0){
          pthread_cond_wait(&(read_write_lock->reader_gate), &(read_write_lock->mutex));
        }
        read_write_lock->reader_count++;
        pthread_mutex_unlock(&(read_write_lock->mutex));
        //pthread_mutex_lock()

        if (read_write_lock->writer_count != 0) {
            printf("Writer found while attempting to read\n");
            error_found = 1;
        }
        curr_readers = read_write_lock->reader_count;
        pthread_mutex_lock(&max_mutex);
        if (curr_readers > max_concurrent_readers) {
            max_concurrent_readers = curr_readers;
        }
        pthread_mutex_unlock(&max_mutex);
#ifdef DEBUG
        printf("[Reader: #%d, Loop: %d] ", tid, i);
#endif
        printf("Reader %d reads: %d\n", tid, value);
        usleep(100);

        /*pthread_mutex_lock(&(read_write_lock->mutex));
        read_write_lock->reader_count--;
        pthread_mutex_unlock(&(read_write_lock->mutex));*/
        //reader_release(read_write_lock);
        pthread_mutex_lock(&(read_write_lock->mutex));
        read_write_lock->reader_count--;
        if(read_write_lock->reader_count == 0 && read_write_lock->writer_wait_count > 0){
          pthread_cond_signal(&(read_write_lock->writer_gate));
        }
        pthread_mutex_unlock(&(read_write_lock->mutex));
    }
    pi = malloc(sizeof(int));
    if (pi == NULL) {
        printf("Pointer allocation failed.\n");
        exit(1);
    }
    *pi = error_found;
    pthread_exit((void*)pi);
}
