
/**
 * @file kernel.h
 * @brief This is the header file for the OS kernel by Yann Bolliger
 * that describes the overall definitions and the model of processes that is
 * used throughout the entire system.
 *
 * @date April 2018
 * @author Yann Bolliger
 */

/**
 * KERNEL
 *
 * This kernel consists of severel modules which try to abstract different
 * aspects of the kernel and provide a clean interface for the other modules
 * and users. All collections/lists/tables of objects are managed by their
 * respective model and the user shall only interact with the methods and
 * the table/list entries.
 *
 * The main entry point of the kernel are the lo- and hilevel handlers.
 * Hilevel handles all the system calls and interrupts. It is therefore the
 * principal user of the described modules.
 *
 * The following modules structure this kernel:
 * - sched
 *    - sched_bfs
 *    - process
 * - mem
 * - io
 *    - pipe
 *
 * Their documentation can be found in the respective header files.
 *
 */

#ifndef __KERNEL_H
#define __KERNEL_H

// import constants and pid_t
#include "../user/libc.h"


/**
 * LIMITS
 *
 * inspired by POSIX <limits.h>
 */

/**
 * The maximum number of processes that the system can run simultaneously.
 * According to the POSIX standard.
 */
#define PROCESS_MAX (256)

/**
 * The maximum number of files a process can have opened simultaneously.
 * According to the POSIX standard.
 */
#define OPEN_MAX (64)

/**
 * ERROR CODES
 *
 * inspired by POSIX <errno.h>
 */
#define ERROR_CODE (-1)

/**
 * Fatal error codes require the system to be restarted.
 */
#define FATAL_CODE (-10)

/**
 * PROCESS MODEL
 *
 * The process model is inspired by the lectures.
 * A process is modelled by its status, its exection context, its opened files,
 * and some more attributes relating to scheduling and priority.
 *
 * Each process is uniquely identified by its process ID of type pid_t.
 * There are never two identical PIDs in the lifetime of the system.
 * PIDs are only reused after a restart.
 */

/**
 * All possible states of a process
 *
 * STATUS_INVALID is used as a default to identify PCBs not being used.
 */
typedef enum {
  STATUS_INVALID,
  STATUS_CREATED,
  STATUS_READY,
  STATUS_EXECUTING,
  STATUS_WAITING,
  STATUS_TERMINATED,
} status_t;

/**
 * Execution context type
 * (all the saved registers in svc mode, reverse order)
 */
typedef struct {
  uint32_t cpsr;
  uint32_t pc;
  uint32_t gpr[13];
  uint32_t sp;
  uint32_t lr;
} ctx_t;

/**
 * FILE DESCRIPTORS
 *
 * Inspired by Unix a file can be anything. Though, in this kernel there are
 * only two possibilites: std[in|out|err] or pipes.
 * Nonetheless, this could be extended to support more generic or other types
 * of files.
 *
 * Files are referenced by a process via the file descriptor. The latter is a
 * simple index into the file descriptor table of this process (see below).
 */

/**
 * File descriptor entry for the per-process file descriptor table.
 *
 * This is being referenced to by a file descriptor integer.
 * It saves the mode in which the file is opened and some reference.
 */
typedef struct {
  int mode;     // modes shall be OR'ed (see below)
  void * file;
} fd_t;

#define R_OK (0x01)
#define W_OK (0x10)

/**
 * PCB Process control block type
 *
 * This takes all of the above and yields the described process model.
 * All the PCBs are managed by the process module.
 */
typedef struct {
  pid_t pid;
  status_t status;
  ctx_t ctx;
  uint64_t deadline;
  uint64_t timeslice;
  int user_prio;
  uint32_t stack_page;
  fd_t fd_table[OPEN_MAX];
} pcb_t;


#endif
