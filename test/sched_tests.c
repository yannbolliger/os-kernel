


#include "min_unit.h"
#include "../kernel/sched.h"
#include "../kernel/sched_rq.h"

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

/**
 * Tests
 */

int add_process_rq_test() {
  pid_t pid_test = 10;

  size_t head = global_rq.head;
  size_t tail = global_rq.tail;
  add_process_rq(pid_test, 0, 0);

  _assert_message(
    "Should increase tail but not head",
    global_rq.head == head &&
    global_rq.tail == tail + 1 &&
    global_rq.run_queue[tail].pid == pid_test
    );

  remove_edp_rq();
  return 0;
}

int schedule_process_rq_test() {
  const pid_t pid_test = 10;

  size_t index = global_rq.tail;
  sched_process_rq(pid_test);

  _assert_message(
    "Should get deadline and timeslice",
    global_rq.run_queue[index].pid == pid_test &&
    global_rq.run_queue[index].timeslice == TIME_SLICE &&
    global_rq.run_queue[index].deadline >= global_rq.jiffies + TIME_SLICE
    );

  remove_edp_rq();
  return 0;
}

int add_process_rq_order_test() {

  size_t head = global_rq.head;
  size_t tail = global_rq.tail;
  add_process_rq(10, TIME_SLICE, 10);
  add_process_rq(11, TIME_SLICE, 20);
  add_process_rq(12, TIME_SLICE, 5);

  _assert_message(
    "Deadlines should be ordered increasingly",
    global_rq.edt->deadline == 5 &&
    global_rq.edt->next->deadline == 10 &&
    global_rq.edt->next->next->deadline == 20
    );

  _assert_message(
    "Earliest deadline returns correct entry.",
    earliest_deadline_process()->deadline == 5
    );

  remove_edp_rq();
  remove_edp_rq();
  remove_edp_rq();
  return 0;
}


int all_tests_sched() {
  _verify(add_process_rq_test);
  _verify(schedule_process_rq_test);
  _verify(add_process_rq_order_test);
  return 0;
}
