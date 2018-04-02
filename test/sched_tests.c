


#include "min_unit.h"
#include "../kernel/sched.h"

/**
 * dummy declarations to fulfill sched.c requirements
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
extern void add_process_rq(pid_t, uint64_t, uint64_t);
extern void schedule_process_rq(pid_t);

/**
 * Tests
 */

int add_process_rq_test() {
  const pid_t pid_test = 10;

  add_process_rq(pid_test, 0, 0);

  _assert_message(
    "Should increase tail but not head.",
    global_rq.head == 0 &&
    global_rq.tail == 1 &&
    global_rq.run_queue[0].pid == pid_test
    );

  return 0;
}

int schedule_process_rq_test() {
  const pid_t pid_test = 10;

  schedule_process_rq(pid_test);

  _assert_message(
    "Should get deadline and timeslice tail but not head.",
    global_rq.head == 0 &&
    global_rq.tail == 1 &&
    global_rq.run_queue[0].pid == pid_test
    );

  return 0;
}


int all_tests_sched() {
  _verify(add_process_rq_test);
  return 0;
}
