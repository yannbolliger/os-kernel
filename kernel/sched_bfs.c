
#include "sched_bfs.h"

rq_t global_rq = {0};

pid_t add_process_rq(pid_t pid, uint64_t timeslice, uint64_t deadline) {
  rq_entry_t* new_entry = &global_rq.run_queue[global_rq.tail];
  size_t new_tail = (global_rq.tail + 1) % PROCESS_MAX;

  // check if full
  if (new_tail == global_rq.head) return 0;

  global_rq.tail = new_tail;
  memset(new_entry, 0, sizeof(rq_entry_t));

  new_entry->pid = pid;
  new_entry->deadline = deadline;
  new_entry->timeslice = timeslice;

  return pid;
}

pid_t sched_process_rq(pid_t pid) {
  uint64_t timeslice = TIME_SLICE;
  uint64_t deadline = global_rq.jiffies + timeslice;

  return add_process_rq(pid, timeslice, deadline);
}

rq_entry_t* earliest_deadline_rq() {
  size_t index = global_rq.head;
  rq_entry_t* earliest = NULL;
  uint64_t earliest_deadline = ~0ULL;

  while (index !=  global_rq.tail) {
    rq_entry_t* current = &global_rq.run_queue[index];
    index = (index + 1) % PROCESS_MAX;

    if (current->deadline < earliest_deadline) {
      earliest = current;
      earliest_deadline = current->deadline;
    }
  }
  return earliest;
}

void remove_entry_rq(rq_entry_t* to_remove) {
  // if empty
  if (global_rq.head == global_rq.tail) return;

  // overwrite to_remove in array with first elem of array
  *to_remove = global_rq.run_queue[global_rq.head];

  global_rq.head = (global_rq.head + 1) % PROCESS_MAX;
}

void remove_pid_rq(pid_t pid_to_remove) {
  size_t index = global_rq.head;

  while (index != global_rq.tail) {
    rq_entry_t* current = &global_rq.run_queue[index];
    index = (index + 1) % PROCESS_MAX;

    if (current->pid == pid_to_remove) {
      remove_entry_rq(current);
      break;
    }
  }
}

void sched_tick_rq() {
  global_rq.jiffies++;
}
