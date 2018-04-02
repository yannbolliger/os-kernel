

#include "sched.h"


extern void main_P3();
extern void main_P4();
extern uint32_t tos_P3;
extern uint32_t tos_P4;

pcb_table_t pcb_table = {0};


void run_next_process(ctx_t* ctx) {
  pid_t edp = earliest_deadline_pid_rq();
  pcb_t* to_dispatch_pcb = &pcb_table.pcb[edp];

  remove_earliest_for_dispatch_rq(to_dispatch_pcb);
  pcb_table.executing_pid = dispatch_process(to_dispatch_pcb, ctx);
}

/**
 * Initialise PCBs representing processes stemming from execution of
 * the two user programs.  Note in each case that
 *
 * - the CPSR value of 0x50 means the processor is switched into USR
 *   mode, with IRQ interrupts enabled, and
 * - the PC and SP values matche the entry point and top of stack.
 */

void sched_rst(ctx_t* ctx) {

  // reset PCB table
  memset(&pcb_table, 0, sizeof(pcb_table_t));

  // populate with two processes
  pcb_table.pcb_size = 1;

  pcb_table.pcb[0].pid      = 0;
  pcb_table.pcb[0].ctx.cpsr = 0x50;
  pcb_table.pcb[0].ctx.pc   = (uint32_t)(&main_P3);
  pcb_table.pcb[0].ctx.sp   = (uint32_t)(&tos_P3);
  sched_process_rq(0);

  pcb_table.pcb[1].pid      = 1;
  pcb_table.pcb[1].ctx.cpsr = 0x50;
  pcb_table.pcb[1].ctx.pc   = (uint32_t)(&main_P4);
  pcb_table.pcb[1].ctx.sp   = (uint32_t)(&tos_P4);
  sched_process_rq(1);

  run_next_process(ctx);
  return;
}

void sched_tick() {
  pcb_table.pcb[pcb_table.executing_pid].timeslice--;
  sched_tick_rq();
  return;
}

/**
 * A process needs to be preempted and rescheduled
 * if one of the following happens:
 * - it's timeslice is over
 */
int sched_need_resched() {
  return pcb_table.pcb[pcb_table.executing_pid].timeslice == 0;
}

/**
 * Scheduler main function
 * Preempt runnning process and dispatch the next process with the highest prio.
 */
void sched(ctx_t* ctx) {
  pcb_t* exec = &pcb_table.pcb[pcb_table.executing_pid];

  if (exec->timeslice == 0) sched_process_rq(exec->pid);
  else add_process_rq(exec->pid, exec->timeslice, exec->deadline);

  pid_t edp = earliest_deadline_pid_rq();
  if (edp != exec->pid) {
    interrupt_process(exec, ctx);
  }

  run_next_process(ctx);
  return;
}
