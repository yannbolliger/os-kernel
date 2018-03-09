
#ifndef __HILEVEL_H
#define __HILEVEL_H

// Include functionality relating to newlib (the standard C library).

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Include device definitions
#include "GIC.h"
#include "PL011.h"
#include "SP804.h"
#include "libc.h"

// Include functionality relating to the   kernel.
#include "lolevel.h"
#include "int.h"

// interval for pre-emptive multitasking: 2^20 ticks
#define TIMER_INTERVAL_TICKS (0x00100000);


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
