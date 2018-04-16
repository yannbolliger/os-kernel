

#ifndef __PROCESS_H
#define __PROCESS_H

#include <string.h>
#include "kernel.h"
#include "io.h"
#include "pipe.h"


typedef struct {
  pcb_t pcb[PROCESS_MAX];
  pid_t max_pid;
  pcb_t* executing_pcb;
} pcb_table_t;

void   pcb_rst();
pcb_t* pcb_of(pid_t pid);

pid_t executing_process();
pid_t create_process(uint32_t cpsr, uint32_t pc);
pid_t fork_process(pcb_t* const parent);
int   destroy_process(pid_t pid);
pid_t interrupt_executing_process(ctx_t* ctx);
pid_t dispatch_process(pid_t pid, ctx_t* ctx);
pcb_t* update_pcb_of_executing_process(ctx_t* ctx);

#endif
