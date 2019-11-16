/*************************************
 * Lab 4 Exercise 4
 * Name: Lin Yuyang
 * Student No: A0207526H
 * Lab Group: 09
 *************************************/

// You can modify anything in this file.
// Unless otherwise stated, a line of code being present in this template
//  does not imply that it is correct/necessary!
// You can also add any global or local variables you need (e.g. to implement your page replacement algorithm).

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include<time.h>

#include "api.h"

int find_empty_frame(int * inframe_page, int frame_num) {
  for (int i=0; i<frame_num; i++) {
    if (inframe_page[i] == -1)
      return i;
  }
  return -1;
}

int next(int frame_index, int frame_num) {
  return (frame_index + 1) % frame_num;
}

int search_page(int* inframe_page, int requested_page, int frame_num) {
  for (int i=0; i<frame_num; i++) {
    if (inframe_page[i] == requested_page)
      return 1;
  }
  return -1;
}

void os_run(int initial_num_pages, page_table *pg_table){
    // The main loop of your memory manager
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);
    sigaddset(&signals, SIGUSR2);

    srand(time(0));

    int frame_num = 1<<FRAME_BITS;

    int * inframe_page;
    inframe_page = malloc(sizeof(int) * frame_num);
    for (int i=0; i<frame_num; i++) {
      inframe_page[i] = -1;
    }

    int victim_page = -1;

    int total_page_num = 1<<PAGE_BITS;

    int* total_pages;
    total_pages = malloc(sizeof(int) * total_page_num);

    for (int i=0; i<total_page_num; i++) {
      total_pages[i] = -1;
    }

    while (1) {
        siginfo_t info;
        int signum = sigwaitinfo(&signals, &info);

        // retrieve the index of the page that the user program wants, or -1 if the user program has terminated
        if (signum == SIGUSR1) {
          int const requested_page = info.si_value.sival_int;
          if (requested_page == -1) break;

          if (requested_page > initial_num_pages - 1 &&
            search_page(total_pages, requested_page, total_page_num)==-1) {
            //segment fault
            union sigval reply_value;
            reply_value.sival_int = 1;
            sigqueue(info.si_pid, SIGCONT, reply_value);
          } else {
            //no segment fault
              int empty_frame_index = find_empty_frame(inframe_page, frame_num);
              if (empty_frame_index != -1) {
                //has empty frame
                if (search_page(total_pages, requested_page, total_page_num)==-1){
                  disk_create(requested_page);
                  total_pages[requested_page] = requested_page;
                }
                disk_read(empty_frame_index, requested_page);
                inframe_page[empty_frame_index] = requested_page;
                pg_table->entries[requested_page].frame_index = empty_frame_index;
                pg_table->entries[requested_page].valid = 1;
                //pg_table->entries[requested_page].referenced = 0;
                if (victim_page == -1) victim_page = requested_page;
              } else {
                //page table is full
                //decrease ref until find victim page
                while (pg_table->entries[victim_page].referenced == 1) {
                  pg_table->entries[victim_page].referenced = 0;
                  int frame_index = pg_table->entries[victim_page].frame_index;
                  victim_page = inframe_page[next(frame_index, frame_num)];
                }
                //find victim page
                int replace_frame_index = pg_table->entries[victim_page].frame_index;
                if (pg_table->entries[victim_page].dirty == 1) {
                  disk_write(replace_frame_index, victim_page);
                  pg_table->entries[victim_page].dirty = 0;
                }
                pg_table->entries[victim_page].valid = 0;
                if (search_page(total_pages, requested_page, total_page_num)==-1){
                  disk_create(requested_page);
                  total_pages[requested_page] = requested_page;
                }
                disk_read(replace_frame_index, requested_page);
                inframe_page[replace_frame_index] = requested_page;
                pg_table->entries[requested_page].valid = 1;
                pg_table->entries[requested_page].frame_index = replace_frame_index;
                //update victim page
                victim_page = inframe_page[next(replace_frame_index, frame_num)];

              }

            // tell the MMU that we are done updating the page table
            union sigval reply_value;
            reply_value.sival_int = 0;
            sigqueue(info.si_pid, SIGCONT, reply_value);
          } // end of else


        } else if (signum == SIGUSR2) {
          int const unmap_page = info.si_value.sival_int;
          if (unmap_page == -1) {
            //mmap
            int map_page = rand() % (1<<PAGE_BITS - 1 - initial_num_pages) + initial_num_pages;
            disk_create(map_page);
            total_pages[map_page] = map_page;
            union sigval reply_value;
            reply_value.sival_int = map_page;
            sigqueue(info.si_pid, SIGCONT, reply_value);
          } else {
            //munmap
            if (search_page(total_pages, unmap_page, total_page_num) == -1) {
              union sigval reply_value;
              reply_value.sival_int = 0;
              sigqueue(info.si_pid, SIGCONT, reply_value);
            } else {
              pg_table->entries[unmap_page].valid = 0;
              disk_delete(unmap_page);
              total_pages[unmap_page] = -1;
              union sigval reply_value;
              reply_value.sival_int = 0;
              sigqueue(info.si_pid, SIGCONT, reply_value);
            }

          }
        }


    }
}
