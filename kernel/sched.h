
/**
 * @file sched.h
 * @brief Manages the hilevel process handling. This includes the logic of all
 * system calls that involve process scheduling, dispatching, destroying etc.
 * Therefore it acts as main interface for hilevel handler.
 *
 * @date April 2018
 * @author Yann Bolliger
 */

#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <string.h>

#include "kernel.h"
#include "io.h"
#include "process.h"
#include "sched_bfs.h"

/**
 * TIMERS
 *
 * (inspired by Linux)
 *
 * The timers of the Cortex-A8 ["are clocked by a 32.768kHz reference"]
 * (http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0417d/index.html).
 *
 * In accordance to earlier Linux schedulers, the timer should fire 100 times a
 * second and therefore:
 */

#define HZ                   (100)
#define TIMER_LOAD_MAX       (2^15)
#define TIMER_INTERVAL_TICKS (TIMER_LOAD_MAX/HZ)

/**
 * PRIORITIES
 *
 * In accordance to Linux a process can have a user given priority (nice value)
 * in the following range. (max stands for highest priority, not highest value)
 */
#define MAX_USER_PRIO        (-20)
#define MIN_USER_PRIO        (20)

/**
 * INTERFACE
 */

int  sched_rst(ctx_t* ctx);
void sched_tick();
void sched_fork(ctx_t* ctx);
void sched_exec(ctx_t* ctx);
int  sched_terminate(pid_t pid, ctx_t* ctx);
int  sched_need_resched();
int  sched(ctx_t* ctx);
void sched_nice(pid_t pid, int prio);

#endif
