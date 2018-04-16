
#include "min_unit.h"
#include "../kernel/sched_bfs.h"

extern rq_t global_rq;

/**
 * Helpers
 */
void dirty_reset_of_queue() {
  global_rq.head = 0;
  global_rq.tail = 0;
}

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

  dirty_reset_of_queue();
  return 0;
}

int add_process_when_full_test() {
  _assert_message(
    "Head and tail are zero at start",
    global_rq.head == 0 &&
    global_rq.tail == 0
    );

  for (int i = 0; i < PROCESS_MAX; i++) {
    add_process_rq(100, 0, 0);
  }

  _assert_message(
    "Head and tail when full",
    global_rq.head == 0 &&
    global_rq.tail == PROCESS_MAX - 1
    );

  size_t head = global_rq.head;
  size_t tail = global_rq.tail;
  _assert_message(
    "Returns zero if run_queue is full",
    add_process_rq(101, 0, 0) == 0 &&
    head == global_rq.head &&
    tail == global_rq.tail
    );

  remove_entry_rq(earliest_deadline_rq());

  _assert_message(
    "Can again add element after remove one",
    global_rq.head == 1 &&
    add_process_rq(10000, 0, 0) != 0
    );

  _assert_message(
    "Index wraps around end of array",
    global_rq.tail == 0
    );

  dirty_reset_of_queue();
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

  dirty_reset_of_queue();
  return 0;
}

int add_process_rq_order_test() {

  size_t head = global_rq.head;
  size_t tail = global_rq.tail;
  add_process_rq(10, TIME_SLICE, 10);
  add_process_rq(11, TIME_SLICE, 20);
  add_process_rq(12, TIME_SLICE, 5);

  _assert_message(
    "Earliest deadline returns correct entry.",
    earliest_deadline_rq()->deadline == 5
    );

  remove_entry_rq(earliest_deadline_rq());

  _assert_message(
    "Earliest deadline returns correct entry.",
    earliest_deadline_rq()->deadline == 10
    );
  remove_entry_rq(earliest_deadline_rq());

  _assert_message(
    "Earliest deadline returns correct entry.",
    earliest_deadline_rq()->deadline == 20
    );
  remove_entry_rq(earliest_deadline_rq());

  return 0;
}


int all_tests_sched_bfs() {
  _verify(add_process_rq_test);
  _verify(add_process_when_full_test);
  _verify(schedule_process_rq_test);
  _verify(add_process_rq_order_test);
  return 0;
}
