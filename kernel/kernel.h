

#ifndef __KERNEL_H
#define __KERNEL_H

#include <stdint.h>

#include "libc.h"

/**
 * KERNEL
 * Main shared interface for kernel-wide definitions
 */

/**
 * The maximum number of processes that the system can run simultaneously.
 */
#define PROCESS_MAX (256)

/**
 * The maximum number of files a process can have opened simultaneously.
 * According to the POSIX standard.
 */
#define OPEN_MAX (32)

#define ERROR_CODE (-1)

/**
 * Types
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

typedef struct {
  const int fd;
  int mode;
  void * file;
} fd_t;

// PCB Process control block type
typedef struct {
  pid_t pid;
  status_t status;
  ctx_t ctx;
  uint64_t deadline;
  uint64_t timeslice;
  uint32_t mem_base_addr;
  fd_t fd_table[OPEN_MAX];
} pcb_t;


#endif
