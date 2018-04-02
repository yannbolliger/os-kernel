

#ifndef __SCHEDULER_RQ_H
#define __SCHEDULER_RQ_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "sched.h"

typedef struct _rq_entry_t {
  pid_t pid;
  uint64_t deadline;
  uint64_t timeslice;
  struct _rq_entry_t* next;
} rq_entry_t;

typedef struct {
  rq_entry_t run_queue[PCB_TABLE_SIZE];
  size_t head;
  size_t tail;
  uint64_t jiffies;
  rq_entry_t* edt;
} rq_t;


void add_process_rq(pid_t pid, uint64_t timeslice, uint64_t deadline);
void sched_process_rq(pid_t pid);
rq_entry_t* earliest_deadline_process();
void remove_edp_rq();
void sched_rq_tick();

#endif
