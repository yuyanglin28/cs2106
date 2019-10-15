/*************************************
 * Lab 3 Exercise 3
 * Name: Lin Yuyang
 * Student No: A0207526H
 * Lab Group: 09
 *************************************/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "traffic_synchronizer.h"

#define ensure_successful_malloc(ptr)                           \
    if (ptr == NULL) {                                          \
        printf("Memory allocation unsuccessful for" #ptr "\n"); \
        exit(1);                                                \
    }

//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;

typedef struct
{
  sem_t state;
} seg_state_stuct;

sem_t car_limit;
seg_state_stuct *seg_state;
int *car_candidate;

void initialise()
{
  sem_init(&car_limit, 0, num_of_segments-1);

  seg_state = malloc (sizeof (seg_state_stuct) * num_of_segments);
  ensure_successful_malloc(seg_state);
  for (int i=0; i<num_of_segments; i++)
    sem_init(&seg_state[i].state, 0, 1);

  car_candidate = malloc(sizeof(int) * num_of_segments);
  ensure_successful_malloc(car_candidate);
  for (int i=0; i<num_of_segments; i++)
    car_candidate[i] = 0;
}

void cleanup()
{
  sem_destroy(&car_limit);
  for (int i=0; i<num_of_segments; i++)
    sem_destroy(&seg_state[i].state);
  free(seg_state);
}

void* car(void* car)
{
    car_struct * one_car;

    one_car = (car_struct*)car;

    sem_wait(&car_limit);
    sem_wait(&seg_state[one_car->entry_seg].state);
    enter_roundabout(one_car);
    while(car_candidate[one_car->entry_seg] != 0);

    while(one_car->current_seg != one_car->exit_seg){
      sem_wait(&seg_state[NEXT(one_car->current_seg, num_of_segments)].state);
      move_to_next_segment(one_car);
      if (car_candidate[PREV(one_car->current_seg, num_of_segments)] != 0)
         car_candidate[PREV(one_car->current_seg, num_of_segments)] -- ;
      car_candidate[one_car->current_seg] ++ ;
      sem_post(&seg_state[PREV(one_car->current_seg, num_of_segments)].state);

    }
    car_candidate[one_car->current_seg] --;
    exit_roundabout(one_car);

    sem_post(&seg_state[one_car->exit_seg].state);

    sem_post(&car_limit);


    pthread_exit(0);
}
