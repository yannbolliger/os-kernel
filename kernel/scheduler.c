


#include "scheduler.h"

extern void main_P3();
extern void main_P4();
extern uint32_t tos_P3;
extern uint32_t tos_P4;

pcb_table_t pcb_table = {0};
rq_t global_rq = {0};

void add_process_rq(pid_t pid, uint64_t deadline, uint64_t timeslice) {
  pcb_table.pcb[pid].status = STATUS_READY;

  rq_entry_t new_entry = {0};
  new_entry.pid = pid;
  new_entry.deadline = deadline;
  new_entry.timeslice = timeslice;

  global_rq.run_queue[global_rq.tail] = new_entry;
  global_rq.tail = (global_rq.tail + 1) % PCB_TABLE_SIZE;

  return;
}

void interrupt_process (pid_t pid, ctx_t* ctx) {
  pcb_t* interrupted = &pcb_table.pcb[pid];

  memcpy(&interrupted->ctx, ctx, sizeof(ctx_t));
  //enter_ready_queue_process(pid);

  return;
}

void dispatch_process (pid_t pid, ctx_t* ctx) {
  pcb_t* dispatched = &pcb_table.pcb[pid];

  memcpy(ctx, &dispatched->ctx, sizeof(ctx_t));
  dispatched->status = STATUS_EXECUTING;
  pcb_table.executing_pid = pid;

  return;
}

pid_t next_process_ready () {
  /*
     if (ready_queue_head == ready_queue_tail) {
     return -1;
     }

     pid_t next = ready_queue[ready_queue_head];
     ready_queue_head = (ready_queue_head + 1) % PCB_TABLE_SIZE;

     return next;
   */
  return 0;
}

/**
 * Initialise PCBs representing processes stemming from execution of
 * the two user programs.  Note in each case that
 *
 * - the CPSR value of 0x50 means the processor is switched into USR
 *   mode, with IRQ interrupts enabled, and
 * - the PC and SP values matche the entry point and top of stack.
 */

void scheduler_rst(ctx_t* ctx) {

  // reset PCB table
  memset(&pcb_table, 0, sizeof(pcb_table_t));

  // populate with two processes
  pcb_table.pcb_size = 2;

  pcb_table.pcb[0].pid      = 0;
  pcb_table.pcb[0].ctx.cpsr = 0x50;
  pcb_table.pcb[0].ctx.pc   = (uint32_t)(&main_P3);
  pcb_table.pcb[0].ctx.sp   = (uint32_t)(&tos_P3);
  //enter_ready_queue_process(0);

  pcb_table.pcb[1].pid      = 1;
  pcb_table.pcb[1].ctx.cpsr = 0x50;
  pcb_table.pcb[1].ctx.pc   = (uint32_t)(&main_P4);
  pcb_table.pcb[1].ctx.sp   = (uint32_t)(&tos_P4);
  //enter_ready_queue_process(1);

  dispatch_process(next_process_ready(), ctx);

  return;
}

void scheduler(ctx_t* ctx) {

  interrupt_process(pcb_table.executing_pid, ctx);
  dispatch_process(next_process_ready(), ctx);

  return;
}
