
#include "sched_bfs.h"

rq_t global_rq = {0};

int add_process_rq(pcb_t* pcb) {
  if (global_rq.tail == PROCESS_MAX || NULL == pcb) return ERROR_CODE;

  rq_entry_t* new_entry = &global_rq.run_queue[global_rq.tail++];

  new_entry->pid       = pcb->pid;
  new_entry->deadline  = pcb->deadline;
  new_entry->timeslice = pcb->timeslice;

  return 0;
}

int sched_process_rq(pcb_t* pcb) {
  if (NULL == pcb) return ERROR_CODE;

  pcb->timeslice = TIME_SLICE;
  pcb->deadline = global_rq.jiffies +TIME_SLICE * prio_ratio[pcb->user_prio];

  return add_process_rq(pcb);
}

void remove_entry_rq(rq_entry_t* to_remove) {
  if (NULL == to_remove || global_rq.tail == 0) return;

  // overwrite to_remove in array with first last of array
  *to_remove = global_rq.run_queue[--global_rq.tail];
  memset(&global_rq.run_queue[global_rq.tail], 0, sizeof(rq_entry_t));
}

pid_t pop_earliest_deadline_rq() {
  if (global_rq.tail == 0) return 0;

  rq_entry_t* earliest = NULL;
  uint64_t earliest_deadline = ~0ULL;

  for (size_t i = 0; i < global_rq.tail; i++) {
    rq_entry_t* current = &global_rq.run_queue[i];

    if (current->deadline < earliest_deadline) {
      earliest = current;
      earliest_deadline = current->deadline;
    }
  }
  pid_t pid = earliest->pid;
  remove_entry_rq(earliest);
  return pid;
}

void sched_tick_rq() {
  global_rq.jiffies++;
}
