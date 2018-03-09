


#include "scheduler.h"

extern void main_P3();
extern void main_P4();
extern uint32_t tos_P3;
extern uint32_t tos_P4;

pcb_t pcb[2];
int executing = 0;

/**
 * Initialise PCBs representing processes stemming from execution of
 * the two user programs.  Note in each case that
 *
 * - the CPSR value of 0x50 means the processor is switched into USR
 *   mode, with IRQ interrupts enabled, and
 * - the PC and SP values matche the entry point and top of stack.
 */

void scheduler_rst(ctx_t* ctx) {

  memset(&pcb[0], 0, sizeof(pcb_t));
  pcb[0].pid      = 1;
  pcb[0].status   = STATUS_READY;
  pcb[0].ctx.cpsr = 0x50;
  pcb[0].ctx.pc   = (uint32_t) (&main_P3);
  pcb[0].ctx.sp   = (uint32_t) (&tos_P3);

  memset(&pcb[1], 0, sizeof(pcb_t));
  pcb[1].pid      = 2;
  pcb[1].status   = STATUS_READY;
  pcb[1].ctx.cpsr = 0x50;
  pcb[1].ctx.pc   = (uint32_t) (&main_P4);
  pcb[1].ctx.sp   = (uint32_t) (&tos_P4);


  memcpy(ctx, &pcb[0].ctx, sizeof(ctx_t));
  pcb[0].status = STATUS_EXECUTING;
  executing = 0;

  return;
}

void scheduler(ctx_t* ctx) {

  if (0 == executing) {
    memcpy(&pcb[0].ctx, ctx, sizeof(ctx_t));       // preserve P_1
    pcb[0].status = STATUS_READY;                  // update   P_1 status
    memcpy(ctx, &pcb[1].ctx, sizeof(ctx_t));       // restore  P_2
    pcb[1].status = STATUS_EXECUTING;              // update   P_2 status
    executing = 1;                                 // update   index => P_2
  }
  else if (1 == executing) {
    memcpy(&pcb[1].ctx, ctx, sizeof(ctx_t));       // preserve P_2
    pcb[1].status = STATUS_READY;                  // update   P_2 status
    memcpy(ctx, &pcb[0].ctx, sizeof(ctx_t));       // restore  P_1
    pcb[0].status = STATUS_EXECUTING;              // update   P_1 status
    executing = 0;                                 // update   index => P_1
  }

  return;
}
