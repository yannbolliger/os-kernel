
#include "sched_rq.h"

rq_t global_rq = {0};

void add_process_rq(pid_t pid, uint64_t timeslice, uint64_t deadline) {

  // store entry physically in array
  rq_entry_t* new_entry = &global_rq.run_queue[global_rq.tail];
  new_entry->pid = pid;
  new_entry->deadline = deadline;
  new_entry->timeslice = timeslice;
  new_entry->next = NULL;

  // link entry in queue according to ordered deadlines
  if (global_rq.edt == NULL) {
    global_rq.edt = new_entry;
  }
  else {
    rq_entry_t* next = global_rq.edt;
    rq_entry_t* prev = NULL;
    while (next != NULL && next->deadline < deadline) {
      prev = next;
      next = next->next;
    }
    new_entry->next = next;

    if (prev == NULL) global_rq.edt = new_entry;
    else prev->next = new_entry;
  }

  global_rq.tail = (global_rq.tail + 1) % MAX_NUMBER_PROCESSES;
  return;
}

void sched_process_rq(pid_t pid) {
  uint64_t timeslice = TIME_SLICE;
  uint64_t deadline = global_rq.jiffies + timeslice;

  add_process_rq(pid, timeslice, deadline);
  return;
}

pid_t earliest_deadline_pid_rq() {
  return global_rq.edt->pid;
}

void remove_edp_rq() {
  rq_entry_t* to_remove = global_rq.edt;

  // set new head of queue
  global_rq.edt = to_remove->next;
  // overwrite to_remove in array with first elem of array
  *to_remove = global_rq.run_queue[global_rq.head];

  global_rq.head = (global_rq.head + 1) % MAX_NUMBER_PROCESSES;
}

pid_t remove_earliest_for_dispatch_rq(pcb_t* pcb) {
  rq_entry_t* edt = global_rq.edt;

  pcb->pid       = edt->pid;
  pcb->timeslice = edt->timeslice;
  pcb->deadline  = edt->deadline;

  remove_edp_rq();
  return pcb->pid;
}

void sched_tick_rq() {
  global_rq.jiffies++;
}
