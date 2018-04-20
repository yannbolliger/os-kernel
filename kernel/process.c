

#include "process.h"

pcb_table_t pcb_table;

static inline size_t index_of(pid_t pid) {
  return (pid - 1 + PROCESS_MAX) % PROCESS_MAX;
}

void pcb_rst() {
  memset(&pcb_table, 0, sizeof(pcb_table_t));
}

pid_t executing_process() {
  if (pcb_table.executing_pcb == NULL) return 0;
  else return pcb_table.executing_pcb->pid;
}

/**
 * Returns the PCB for a given PID iff the process has been created and is not
 * terminated yet e.g. for "non-zombie" processes.
 *
 * @param pid_t valid PID
 * @returns pcb_t* pointer to the PCB or NULL if not found
 */
pcb_t* pcb_of(pid_t pid) {
  if (pid <= pcb_table.max_pid || pid > 0) {
    pcb_t* pcb = &pcb_table.pcb[index_of(pid)];

    if (pcb->status != STATUS_INVALID && pcb->pid == pid) return pcb;
  }
  return NULL;
}

pid_t get_next_pid() {
  size_t index = 0;
  pcb_t* next_pcb = NULL;

  do  {
    index++;
    next_pcb = &pcb_table.pcb[index_of(pcb_table.max_pid + index)];
  } while (index < PROCESS_MAX && next_pcb->status != STATUS_INVALID);

  if (next_pcb->status == STATUS_INVALID) return pcb_table.max_pid + index;
  else return 0;
}

pid_t create_process(uint32_t cpsr, uint32_t pc) {
  pid_t pid = get_next_pid();
  if (pid == 0) return 0;

  // check if memory available
  uint32_t stack_page_number = page_allocate();
  if (stack_page_number == 0) return 0;

  // reset pcb
  pcb_t* pcb = &pcb_table.pcb[index_of(pid)];
  memset(pcb, 0, sizeof(pcb_t));

  // set PID
  pcb->pid          = pid;
  pcb_table.max_pid = pid;

  // set execution context
  pcb->ctx.cpsr = cpsr;
  pcb->ctx.pc   = pc;
  // stack grows downwards
  pcb->ctx.sp   = TOS_USER;

  pcb->status   = STATUS_READY;
  pcb->stack_page = stack_page_number;

  return pcb->pid;
}

/**
 * Destroy a process and remove it from the PCB table.
 *
 */
int _destroy_process(pcb_t* pcb_to_remove) {
  if (pcb_to_remove == NULL ||
      pcb_to_remove->pid == executing_process()) return ERROR_CODE;

  // free stack memory
  int err = page_deallocate(pcb_to_remove->stack_page);
  if (err) return ERROR_CODE;

  // close all open fds
  for (size_t i = 0; i < OPEN_MAX; i++) {
    io_close(pcb_to_remove->pid, i);
  }

  // discard pcb
  pcb_to_remove->status = STATUS_INVALID;
  return 0;
}

int destroy_process(pid_t to_destroy) {
  return _destroy_process(pcb_of(to_destroy));
}

pid_t fork_process(pcb_t* const parent) {
  if (NULL == parent) return 0;

  pid_t child_pid = create_process(parent->ctx.cpsr, parent->ctx.pc);
  pcb_t* child = pcb_of(child_pid);
  if (child_pid == 0 || NULL == child) return 0;

  // replicate context
  child->ctx = parent->ctx;

  // set return value and different sp
  child->ctx.gpr[0] = 0;

  int err = page_copy(parent->stack_page, child->stack_page);
  if (err) {
    _destroy_process(child);
    return 0;
  }

  // copy all fds
  memcpy(child->fd_table, parent->fd_table, OPEN_MAX*sizeof(fd_t));

  for (size_t i = STDERR_FILENO + 1; i < OPEN_MAX; i++) {
    pipe_fork(child->fd_table[i].file);
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
  pcb_table.executing_pcb = NULL;

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
