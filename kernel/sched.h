

#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "kernel.h"
#include "io.h"
#include "process.h"
#include "sched_bfs.h"

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

#define MAX_USER_PRIO        (-20)
#define MIN_USER_PRIO        (20)

/**
 * Scheduler calls
 */

int  sched_rst(ctx_t* ctx);
void sched_tick();
void sched_fork(ctx_t* ctx);
int  sched_terminate(pid_t pid, ctx_t* ctx);
int  sched_need_resched();
int  sched(ctx_t* ctx);

#endif
