
/**
 * @file sched_bfs.h
 * @brief Implements the brainfuck scheduling algorithm and manages
 * the run queue and the selection of the next process to run.
 *
 * @date April 2018
 * @author Yann Bolliger
 */


/**
 * BFS
 *
 * The brainfuck scheduler was chosen due to its simplicity and in accordance
 * with the underlying hardware. The kernel will run on an embedded system
 * which is even a single-core configuration. BFS was desiged to fit systems
 * with less than 16 CPUs it is therefore suited for this case.
 * Additionaly, BFS does not rely on complex heuristics in order to find out
 * whether a process is IO-bound or CPU-bound. This makes it so simple.
 * The only disadvantage is the process retrieval which is in O(n), where n
 * is the number of processes in the run queue. However, this kernel only
 * supports up to PROCESS_MAX (256) processes. Therefore, even linear time is
 * feasible.
 *
 * (http://ck.kolivas.org/patches/bfs/bfs-faq.txt)
 */


#ifndef __SCHEDULER_BFS_H
#define __SCHEDULER_BFS_H

#include "kernel.h"
#include "sched.h"

/**
 * Smallest possible time slice each process gets in HZ.
 *
 * According to Con Kolivas, this is the *only tunable* of the BF scheduler.
 */
#define TIME_SLICE     (60)

typedef struct {
  pid_t pid;
  uint64_t deadline;
} rq_entry_t;

typedef struct {
  rq_entry_t run_queue[PROCESS_MAX];
  size_t tail;
  uint64_t jiffies;
} rq_t;

/**
 * INTERFACE
 */


void sched_bfs_rst();
int add_process_rq(pcb_t* pcb);
int sched_process_rq(pcb_t* pcb);
pid_t pop_earliest_deadline_rq();
void sched_tick_rq();
void remove_pid_rq(pid_t pid);

#endif
