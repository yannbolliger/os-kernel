

#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "kernel.h"
#include "process.h"
#include "sched_rq.h"

/**
 * System Constants
 */


/**
 * Type Definitions Scheduler
 */

typedef struct {
  pcb_t pcb[MAX_NUMBER_PROCESSES];
  size_t pcb_size;
  pid_t executing_pid;
} pcb_table_t;

/**
 * Scheduler calls
 */

void sched_rst(ctx_t* ctx);
void sched_tick();
int  sched_need_resched();
void sched(ctx_t* ctx);

#endif
