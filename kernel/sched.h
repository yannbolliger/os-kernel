

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
 * Scheduler calls
 */

void sched_rst(ctx_t* ctx);
void sched_tick();
void sched_fork(ctx_t* ctx);
int  sched_need_resched();
void sched(ctx_t* ctx);

#endif
