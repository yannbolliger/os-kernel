

#include "sched.h"


extern void main_cool_console();

int run_next_process(ctx_t* ctx) {
  pid_t edp = pop_earliest_deadline_rq();

  if (edp == 0) {
    kernel_write_error("No more tasks scheduled. System idle.\n", 38);
    return FATAL_CODE;
  }

  dispatch_process(edp, ctx);
  return 0;
}

void sched_fork(ctx_t* ctx) {
  pcb_t* parent = update_pcb_of_executing_process(ctx);
  pid_t child_pid = fork_process(parent);
  int err = sched_process_rq(pcb_of(child_pid));

  if (!err) {
    ctx->gpr[0] = child_pid;
  }
  else {
    destroy_process(child_pid);

    // return error code to the calling process otherwise
    ctx->gpr[0] = ERROR_CODE;
  }
}

int sched_terminate(pid_t pid_to_remove, ctx_t* ctx) {
  // halt and reschedule current process
  pcb_t* exec = pcb_of(interrupt_executing_process(ctx));

  int err1 = add_process_rq(exec);
  int err2 = destroy_process(pid_to_remove);
  remove_pid_rq(pid_to_remove);

  int fatal = run_next_process(ctx);

  if (fatal) return FATAL_CODE;
  else return err1 || err2;
}

/**
 * Initialise PCBs representing processes stemming from execution of
 * the two user programs.  Note in each case that
 *
 * - the CPSR value of 0x50 means the processor is switched into USR
 *   mode, with IRQ interrupts enabled, and
 * - the PC and SP values matche the entry point and top of stack.
 */

int sched_rst(ctx_t* ctx) {
  pcb_rst();
  sched_bfs_rst();

  pid_t console_pid = create_process(0x50, (uint32_t) (&main_cool_console));

  int err = sched_process_rq(pcb_of(console_pid));
  if (err) {
    kernel_write_error("System could not start console.\n", 32);
    return FATAL_CODE;
  }

  return run_next_process(ctx);
}

void sched_tick() {
  pcb_of(executing_process())->timeslice--;
  sched_tick_rq();
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
int sched(ctx_t* ctx) {
  pcb_t* exec = pcb_of(interrupt_executing_process(ctx));

  // determines whether the call is made from the timer or from yield
  int err = 0;
  if (exec->timeslice == 0) err = sched_process_rq(exec);
  else err = add_process_rq(exec);

  int fatal = run_next_process(ctx);

  if (fatal) return FATAL_CODE;
  else return err;
}

void sched_nice(pid_t pid, int prio) {
  if (prio < MAX_USER_PRIO || prio > MIN_USER_PRIO) return;

  pcb_t* pcb = pcb_of(pid);
  if (NULL == pcb) return;

  pcb->user_prio = prio;
  remove_pid_rq(pid);
  sched_process_rq(pid);
}
