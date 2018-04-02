

#ifndef __PROCESS_H
#define __PROCESS_H

#include <string.h>
#include "kernel.h"

pid_t create_process();
pid_t interrupt_process(pcb_t* interrupted_pcb, ctx_t* ctx);
pid_t dispatch_process(pcb_t* dispatched_pcb, ctx_t* ctx);

#endif
