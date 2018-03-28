


#include "scheduler.h"

extern void main_P3();
extern void main_P4();
extern uint32_t tos_P3;
extern uint32_t tos_P4;

pcb_table_t pcb_table = {0};

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
  pcb_table.pcb[0].pid      = 0;
  pcb_table.pcb[0].status   = STATUS_READY;
  pcb_table.pcb[0].ctx.cpsr = 0x50;
  pcb_table.pcb[0].ctx.pc   = (uint32_t) (&main_P3);
  pcb_table.pcb[0].ctx.sp   = (uint32_t) (&tos_P3);

  pcb_table.pcb[1].pid      = 1;
  pcb_table.pcb[1].status   = STATUS_READY;
  pcb_table.pcb[1].ctx.cpsr = 0x50;
  pcb_table.pcb[1].ctx.pc   = (uint32_t) (&main_P4);
  pcb_table.pcb[1].ctx.sp   = (uint32_t) (&tos_P4);

  pcb_table.pcb_size = 2;

  // set first process to executing state
  memcpy(ctx, &pcb_table.pcb[0].ctx, sizeof(ctx_t));
  pcb_table.pcb[0].status = STATUS_EXECUTING;
  pcb_table.executing_pid = pcb_table.pcb[0].pid;

  return;
}

void interrupt_process(pid_t pid, ctx_t* ctx) {
  pcb_t* interrupted = &pcb_table.pcb[pid];

  memcpy(&interrupted->ctx, ctx, sizeof(ctx_t));
  interrupted->status = STATUS_READY;

  return;
}

void dispatch_process(pid_t pid, ctx_t* ctx) {
  pcb_t* dispatched = &pcb_table.pcb[pid];

  memcpy(ctx, &dispatched->ctx, sizeof(ctx_t));
  dispatched->status = STATUS_EXECUTING;

  return;
}

void scheduler(ctx_t* ctx) {

  interrupt_process(pcb_table.executing_pid, ctx);

  pcb_table.executing_pid = (pcb_table.executing_pid + 1) % pcb_table.pcb_size;
  dispatch_process(pcb_table.executing_pid, ctx);

  return;
}
