

#include "process.h"

pcb_table_t pcb_table = {0};

pid_t executing_process() {
  return pcb_table.executing_pcb->pid;
}

pcb_t* pcb_of(pid_t pid) {
  // fast lookup for executing pcb
  if (pid == executing_process())
    return pcb_table.executing_pcb;
  // slow O(n) lookup for arbitrary processes
  else {
    size_t index = pcb_table.head;
    while (index != pcb_table.tail && pcb_table.pcb[index].pid != pid) {
      index = (index + 1) % MAX_NUMBER_PROCESSES;
    }

    if (pcb_table.pcb[index].pid == pid) return &pcb_table.pcb[index];
    else return NULL;
  }
}

pid_t create_process(uint32_t cpsr, uint32_t pc) {
  pcb_t* pcb = &pcb_table.pcb[pcb_table.tail];
  size_t new_tail = (pcb_table.tail + 1) % MAX_NUMBER_PROCESSES;

  // if the tail would point to the head after insertion, the queue is full
  if (new_tail == pcb_table.head) return 0;

  pcb_table.tail = new_tail;
  memset(pcb, 0, sizeof(pcb_t));

  pcb->pid      = ++pcb_table.max_pid;
  pcb->ctx.cpsr = cpsr;
  pcb->ctx.pc   = pc;
  pcb->status   = STATUS_READY;

  // TODO: sp allocation
  return pcb->pid;
}

pid_t fork_process(pid_t parent_pid) {
  pcb_t* parent = pcb_of(parent_pid);

  // context is copied below
  pid_t child_pid = create_process(0, 0);
  if (child_pid == 0) return 0;

  pcb_t* child = pcb_of(child_pid);
  child->ctx = parent->ctx;

  // return value is 0 for child
  child->ctx.gpr[0] = 0;

  // TODO: copy stack
  return child_pid;
}

pid_t interrupt_executing_process(ctx_t* ctx) {
  pcb_t* interrupted = pcb_of(executing_process());

  memcpy(&interrupted->ctx, ctx, sizeof(ctx_t));
  interrupted->status = STATUS_READY;

  return interrupted->pid;
}

pid_t dispatch_process(pid_t pid, ctx_t* ctx) {
  pcb_t* dispatched = pcb_of(pid);

  memcpy(ctx, &dispatched->ctx, sizeof(ctx_t));
  dispatched->status = STATUS_EXECUTING;

  return dispatched->pid;
}
