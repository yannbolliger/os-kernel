

#include "min_unit.h"
#include "../kernel/process.h"


extern pcb_table_t pcb_table;
ctx_t ctx = {0};

// faking these functions in order to only test process.c
int page_copy(const size_t src, const size_t dst) {
  return 0;
}

int page_deallocate(size_t addr) {
  return 0;
}

size_t stack_page_counter = 0x800;
size_t page_allocate() {
  return stack_page_counter++;
}

int io_close(const pid_t pid, const int fd) {
  return 0;
}

void pipe_fork(pipe_t* pipe) {
  return;
}

void page_swap_stack(pcb_t* next_running_pcb) {
  return;
}

/**
 * Tests
 */
int executing_process_test() {
  _assert_message(
    "Executing process doesn't crash for empty table",
    0 == executing_process()
    );

  pid_t p = create_process(0, 0);

  _assert_message(
    "Executing process is up to date after dispatch.",
    p == dispatch_process(p, &ctx) &&
    executing_process() == p &&
    STATUS_EXECUTING == pcb_of(p)->status
    );

  pcb_rst();
  return 0;
}

int pcb_of_test() {
  _assert_message(
    "PCB of doesn't crash for empty table",
    NULL == pcb_of(100)
    );

  pid_t p1 = create_process(0, 0);

  _assert_message(
    "PCB returns NULL for invalid pid",
    NULL == pcb_of(100)
    );

  pid_t p2 = create_process(0, 0);
  pid_t p3 = create_process(0, 0);
  pid_t p4 = create_process(0, 0);

  _assert_message(
    "PCB of returns correct pcb for valid pid",
    p1 == pcb_of(p1)->pid &&
    p2 == pcb_of(p2)->pid &&
    p3 == pcb_of(p3)->pid &&
    p4 == pcb_of(p4)->pid
    );

  pcb_rst();
  return 0;
}



int create_process_full_test() {
  _assert_message(
    "Create process returns valid pid",
    0 != create_process(0, 0)
    );

  for (int i = 0; i < PROCESS_MAX - 2; i++) {
    create_process(0, 0);
  }

  _assert_message(
    "Create process returns valid pid",
    0 != create_process(0, 0)
    );

  _assert_message(
    "Create process returns 0 if table is full",
    0 == create_process(0, 0)
    );

  pcb_rst();
  return 0;
}

int fork_process_test() {
  pcb_t* parent = pcb_of(create_process(20, 30));
  pcb_t* child  = pcb_of(fork_process(parent));

  _assert_message(
    "Child and parent have same context for most fields",
    parent->ctx.cpsr == child->ctx.cpsr &&
    parent->ctx.lr == child->ctx.lr &&
    parent->ctx.pc == child->ctx.pc
    );

  _assert_message(
    "Child and parent have the same sp but not stack_page",
    parent->ctx.sp == child->ctx.sp &&
    parent->stack_page != child->stack_page
    );

  pcb_rst();
  return 0;
}

int destroy_process_test() {
  // "run one process"
  pid_t pid = create_process(0, 0);
  dispatch_process(pid, &ctx);

  _assert_message(
    "Destroy process cannot destroy executing process.",
    0 != destroy_process(pid)
    );


  pcb_rst();
  return 0;
}

int all_process_tests() {
  _verify(pcb_of_test);
  _verify(executing_process_test);
  _verify(create_process_full_test);
  _verify(fork_process_test);
  _verify(destroy_process_test);
  return 0;
}
