
#include "sched_rq.h"

rq_t global_rq = {0};

void add_process_rq(pid_t pid, uint64_t timeslice, uint64_t deadline) {
  rq_entry_t new_entry = {0};
  new_entry.pid = pid;
  new_entry.deadline = deadline;
  new_entry.timeslice = timeslice;

  global_rq.run_queue[global_rq.tail] = new_entry;
  global_rq.tail = (global_rq.tail + 1) % PCB_TABLE_SIZE;

  return;
}

void sched_process_rq(pid_t pid) {
  uint64_t timeslice = TIME_SLICE;
  uint64_t deadline = global_rq.jiffies + timeslice;

  add_process_rq(pid, timeslice, deadline);
  return;
}

rq_entry_t* earliest_deadline_process() {
  return &global_rq.run_queue[global_rq.head];
}

void remove_edp_rq() {
  global_rq.head = (global_rq.head + 1) % PCB_TABLE_SIZE;
}

void sched_rq_tick() {
  global_rq.jiffies++;
}
