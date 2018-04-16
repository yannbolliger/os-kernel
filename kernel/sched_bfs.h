

#ifndef __SCHEDULER_BFS_H
#define __SCHEDULER_BFS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "kernel.h"
#include "sched.h"


// Smallest possible time slice each process gets [1/100s]
#define TIME_SLICE     (60)


int add_process_rq(pcb_t* pcb);
int sched_process_rq(pcb_t* pcb);
pid_t pop_earliest_deadline_rq();
void sched_tick_rq();

#endif
