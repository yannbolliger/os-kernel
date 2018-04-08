

#include "process.h"

pcb_table_t pcb_table = {0};

pid_t executing_process() {
  if (pcb_table.executing_pcb == NULL) return 0;
  else return pcb_table.executing_pcb->pid;
}

pcb_t* pcb_of(pid_t pid) {
  // fast lookup for executing pcb
  if (pid == executing_process()) {
    return pcb_table.executing_pcb;
  }
  // fast lookup for tail (last created process)
  else if (pid == pcb_table.pcb[pcb_table.tail - 1].pid) {
    return &pcb_table.pcb[pcb_table.tail - 1];
  }
  // slow O(n) lookup for arbitrary processes
  else {
    size_t index = 0;
    while (index < pcb_table.tail && pcb_table.pcb[index].pid != pid) index++;

    if (pcb_table.pcb[index].pid == pid) return &pcb_table.pcb[index];
    else return NULL;
  }
}

pid_t create_process(uint32_t cpsr, uint32_t pc) {
  // check if pcb-stack is full
  if (pcb_table.tail + 1 > MAX_NUMBER_PROCESSES) return 0;

  // check if memory available
  uint32_t stack_base = mem_allocate(1);
  if (stack_base == 0) return 0;

  // reset pcb
  pcb_t* pcb = &pcb_table.pcb[pcb_table.tail++];
  memset(pcb, 0, sizeof(pcb_t));

  // set values
  pcb->pid      = ++pcb_table.max_pid;
  pcb->ctx.cpsr = cpsr;
  pcb->ctx.pc   = pc;
  pcb->ctx.sp   = mem_block_addr_end(stack_base);
  pcb->status   = STATUS_READY;
  pcb->mem_base_addr  = stack_base;

  return pcb->pid;
}

/**
 * Destroy a process and remove it from the PCB table.
 * In order to keep the table dense, the last element of the stack is copied
 * into the empty space.
 */
pid_t destroy_process(pcb_t* pcb_to_remove) {
  if (pcb_to_remove == NULL) return 0;

  // backup pids
  pid_t pid_to_remove = pcb_to_remove->pid;
  pid_t executing_pid = executing_process();

  // free stack memory
  size_t n = mem_deallocate(pcb_to_remove->mem_base_addr, 1);
  if (n != 1) return 0;

  // fill empty space
  *pcb_to_remove = pcb_table.pcb[--pcb_table.tail];
  pcb_t* newly_filled_pcb = pcb_to_remove;
  pcb_to_remove = NULL;

  if (newly_filled_pcb->pid == executing_pid) {
    pcb_table.executing_pcb = newly_filled_pcb;
  }
  return pid_to_remove;
}

pid_t fork_process(pcb_t* const parent) {
  if (NULL == parent) return 0;

  pid_t child_pid = create_process(parent->ctx.cpsr, parent->ctx.pc);
  pcb_t* child = pcb_of(child_pid);
  if (child_pid == 0 || NULL == child) return 0;

  // replicate context
  uint32_t child_sp_backup = child->ctx.sp;
  child->ctx = parent->ctx;

  // set return value and different sp
  child->ctx.gpr[0] = 0;
  child->ctx.sp = child_sp_backup;

  size_t n = mem_copy(parent->mem_base_addr, child->mem_base_addr, 1);
  if (n != 1) {
    destroy_process(child);
    return 0;
  }

  return child_pid;
}

pcb_t* update_pcb_of_executing_process(ctx_t* ctx) {
  pcb_t* executing = pcb_of(executing_process());
  if (NULL == executing) return NULL;

  memcpy(&executing->ctx, ctx, sizeof(ctx_t));
  return executing;
}

pid_t interrupt_executing_process(ctx_t* ctx) {
  pcb_t* interrupted = update_pcb_of_executing_process(ctx);
  interrupted->status = STATUS_READY;

  return interrupted->pid;
}

pid_t dispatch_process(pid_t pid, ctx_t* ctx) {
  pcb_t* dispatched = pcb_of(pid);
  if (NULL == dispatched) return 0;

  memcpy(ctx, &dispatched->ctx, sizeof(ctx_t));
  dispatched->status = STATUS_EXECUTING;
  pcb_table.executing_pcb = dispatched;

  return dispatched->pid;
}
