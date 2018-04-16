


#include "min_unit.h"
#include "../kernel/sched.h"

/**
 * dummy declarations to fulfill sched.c requirements
 */
void main_cool_console() {
  return;
}

void kernel_write_error() {
  return;
}

/**
 * TESTS
 */

int sched_only_one_process_test() {
  // init scheduler
  ctx_t cpu_ctx = {0};
  sched_rst(&cpu_ctx);

  // mock execution
  cpu_ctx.pc = 100;
  pcb_of(executing_process())->timeslice = 0;

  // reschedule same process
  sched(&cpu_ctx);

  _assert_message(
    "After reschduling the context is the same",
    cpu_ctx.pc == 100 &&
    pcb_of(executing_process())->status == STATUS_EXECUTING
    );
  return 0;
}

int all_tests_sched() {
  _verify(sched_only_one_process_test);
  return 0;
}
