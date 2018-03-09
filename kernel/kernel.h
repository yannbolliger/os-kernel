

#ifndef __KERNEL_H
#define __KERNEL_H

// Process IDentifier (PID) type
typedef int pid_t;

// All possible states of a process
typedef enum {
  STATUS_CREATED,
  STATUS_READY,
  STATUS_EXECUTING,
  STATUS_WAITING,
  STATUS_TERMINATED
} status_t;

// Execution context type
// (all the saved registers in svc mode, reverse order)
typedef struct {
  uint32_t cpsr;
  uint32_t pc;
  uint32_t gpr[13];
  uint32_t sp;
  uint32_t lr;
} ctx_t;

// PCB Process control block type
typedef struct {
  pid_t pid;
  status_t status;
  ctx_t ctx;
} pcb_t;

#endif
