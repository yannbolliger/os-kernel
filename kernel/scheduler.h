

#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "kernel.h"

/**
 * System Constants
 */


// Maximum number of processes
#define PCB_TABLE_SIZE (50)

/**
 * Type Definitions Scheduler
 */

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

typedef struct {
  pcb_t pcb[PCB_TABLE_SIZE];
  size_t pcb_size;
  pid_t executing_pid;
} pcb_table_t;

/**
 * Scheduler calls
 */

void scheduler_rst(ctx_t* ctx);
void scheduler(ctx_t* ctx);

#endif
