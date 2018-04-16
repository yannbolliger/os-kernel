
#include "sched_bfs.h"



rq_t global_rq = {0};

void sched_bfs_rst() {
  global_rq.tail = 0;
  global_rq.jiffies = 0;
}

int calculate_deadline(int user_prio) {

  // calculate priority ratio
  const int quotient = 128;
  int ratio = quotient;
  for (size_t i = 0; i < user_prio - MAX_USER_PRIO; i++) {
    ratio = ratio * 11/10;
  }

  return global_rq.jiffies + (TIME_SLICE * ratio) / quotient;
}

int add_process_rq(pcb_t* pcb) {
  if (global_rq.tail == PROCESS_MAX || NULL == pcb) return ERROR_CODE;

  rq_entry_t* new_entry = &global_rq.run_queue[global_rq.tail++];
  memset(new_entry, 0, sizeof(rq_entry_t));

  new_entry->pid       = pcb->pid;
  new_entry->deadline  = pcb->deadline;

  return 0;
}

int sched_process_rq(pcb_t* pcb) {
  if (NULL == pcb) return ERROR_CODE;

  pcb->timeslice = TIME_SLICE;
  pcb->deadline = calculate_deadline(pcb->user_prio);

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

void remove_pid_rq(pid_t pid_to_remove) {
  size_t index = 0;
  rq_entry_t* current = NULL;

  do {
    current = &global_rq.run_queue[index++];
  } while (index < global_rq.tail && current->pid != pid_to_remove);

  if (current->pid == pid_to_remove) remove_entry_rq(current);
}

void sched_tick_rq() {
  global_rq.jiffies++;
}
