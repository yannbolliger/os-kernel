

#include "sched.h"


extern void main_console();


void run_next_process(ctx_t* ctx) {
  pid_t edp = earliest_deadline_pid_rq();
  pcb_t* to_dispatch_pcb = pcb_of(edp);

  remove_earliest_for_dispatch_rq(to_dispatch_pcb);
  dispatch_process(edp, ctx);
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
  pid_t p1 = create_process(0x50, (uint32_t) (&main_console));
  sched_process_rq(p1);

  run_next_process(ctx);
  return;
}

void sched_tick() {
  pcb_of(executing_process())->timeslice--;
  sched_tick_rq();
  return;
}

/**
 * A process needs to be preempted and rescheduled
 * if one of the following happens:
 * - it's timeslice is over
 */
int sched_need_resched() {
  return pcb_of(executing_process())->timeslice == 0;
}

/**
 * Scheduler main function
 * Preempt runnning process and dispatch the next process with the highest prio.
 */
void sched(ctx_t* ctx) {
  pcb_t* exec = pcb_of(executing_process());

  if (exec->timeslice == 0) sched_process_rq(exec->pid);
  else add_process_rq(exec->pid, exec->timeslice, exec->deadline);

  pid_t edp = earliest_deadline_pid_rq();
  if (edp != exec->pid) {
    interrupt_executing_process(ctx);
  }

  run_next_process(ctx);
  return;
}
