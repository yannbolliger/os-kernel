

#include "process.h"


pid_t interrupt_process(pcb_t* interrupted, ctx_t* ctx) {
  memcpy(&interrupted->ctx, ctx, sizeof(ctx_t));
  interrupted->status = STATUS_READY;

  return interrupted->pid;
}

pid_t dispatch_process(pcb_t* dispatched, ctx_t* ctx) {
  memcpy(ctx, &dispatched->ctx, sizeof(ctx_t));
  dispatched->status = STATUS_EXECUTING;

  return dispatched->pid;
}
