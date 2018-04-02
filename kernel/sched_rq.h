

#ifndef __SCHEDULER_RQ_H
#define __SCHEDULER_RQ_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "kernel.h"

// Smallest possible time slice each process gets [1/100s]
#define TIME_SLICE     (60)

typedef struct _rq_entry_t {
  pid_t pid;
  uint64_t deadline;
  uint64_t timeslice;
  struct _rq_entry_t* next;
} rq_entry_t;

typedef struct {
  rq_entry_t run_queue[MAX_NUMBER_PROCESSES];
  size_t head;
  size_t tail;
  uint64_t jiffies;
  rq_entry_t* edt;
} rq_t;


void add_process_rq(pid_t pid, uint64_t timeslice, uint64_t deadline);
void sched_process_rq(pid_t pid);
pid_t earliest_deadline_pid_rq();
pid_t remove_earliest_for_dispatch_rq(pcb_t* pcb);
void sched_tick_rq();

#endif
