/*************************************
 * Lab 4 Exercise 1
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

void os_run(int initial_num_pages, page_table *pg_table){
    // The main loop of your memory manager
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);

    for (int i=0; i!=initial_num_pages; ++i) {
        disk_create(i);
    }

    int frame_num = 1<<FRAME_BITS;

    int * inframe_page;
    inframe_page = malloc(sizeof(int) * frame_num);
    for (int i=0; i<frame_num; i++) {
      inframe_page[i] = -1;
    }

    int victim_page = -1;

    while (1) {
        siginfo_t info;

        sigwaitinfo(&signals, &info);

        // retrieve the index of the page that the user program wants, or -1 if the user program has terminated
        int const requested_page = info.si_value.sival_int;

        if (requested_page == -1) break;

        //process the signal, and update the page table as necessary

          int empty_frame_index = find_empty_frame(inframe_page, frame_num);
          if (empty_frame_index != -1) {
            //has empty frame
            disk_read(empty_frame_index, requested_page);
            inframe_page[empty_frame_index] = requested_page;
            pg_table->entries[requested_page].frame_index = empty_frame_index;
            pg_table->entries[requested_page].valid = 1;
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
            pg_table->entries[victim_page].valid = 0;
            disk_read(replace_frame_index, requested_page);
            inframe_page[replace_frame_index] = requested_page;
            pg_table->entries[requested_page].valid = 1;
            pg_table->entries[requested_page].frame_index = replace_frame_index;
            //update victim page
            victim_page = inframe_page[next(replace_frame_index, frame_num)];
          }
        //}

        // tell the MMU that we are done updating the page table
        union sigval reply_value;
        reply_value.sival_int = 0; // set to 0 if the page is successfully loaded, set to 1 if the page is not mapped to the user process (i.e. segfault)
        sigqueue(info.si_pid, SIGCONT, reply_value);
    }
}
