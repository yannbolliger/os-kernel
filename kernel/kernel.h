

#ifndef __KERNEL_H
#define __KERNEL_H

#include <stdint.h>

/**
 * Timer configuration
 * The timers of the Cortex-A8 ["are clocked by a 32.768kHz reference"]
 * (http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0417d/index.html).
 *
 * In accordance to earlier Linux schedulers, the timer should fire 100 times a
 * second and therefore:
 */

#define HZ                   (100)
#define TIMER_CLOCK          (2^15)
#define TIMER_INTERVAL_TICKS (TIMER_CLOCK/HZ)

#define MAX_NUMBER_PROCESSES (50)

/**
 * Type Definitions Kernel-wide
 */

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
  uint64_t deadline;
  uint64_t timeslice;
} pcb_t;


#endif
