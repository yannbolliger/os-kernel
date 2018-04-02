


#include "min_unit.h"
#include "../kernel/scheduler.h"

/**
 * dummy declarations to fulfill scheduler.c requirements
 */

void main_P3() {
  return;
}
void main_P4() {
  return;
}
uint32_t tos_P3 = 0;
uint32_t tos_P4 = 0;

extern pcb_table_t pcb_table;
extern rq_t global_rq;

/**
 * Tests
 */

int add_entry_to_rq_test() {
  const pid_t pid_test = 10;

  add_process_rq(pid_test);

  _assert_message(
    "Should increase tail but not head.",
    global_rq.head == 0 &&
    global_rq.tail == 1 &&
    global_rq.run_queue[0].pid == pid_test
    );

  return 0;
}


int all_tests_scheduler() {
  _verify(add_entry_to_rq_test);
  return 0;
}
