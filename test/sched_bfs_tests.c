
#include "min_unit.h"
#include "../kernel/sched_bfs.h"

extern rq_t global_rq;


/**
 * Tests
 */

int add_process_rq_test() {
  const pid_t pid_test = 10;
  pcb_t pcb = { .pid = pid_test };

  size_t tail = global_rq.tail;
  add_process_rq(&pcb);

  _assert_message(
    "Should increase tail",
    global_rq.tail == tail + 1 &&
    global_rq.run_queue[tail].pid == pid_test
    );

  sched_bfs_rst();
  return 0;
}

int add_process_when_full_test() {
  pcb_t pcb = {0};

  _assert_message(
    "Tail is zero at start",
    global_rq.tail == 0
    );

  for (int i = 0; i < PROCESS_MAX; i++) {
    add_process_rq(&pcb);
  }

  _assert_message(
    "Head and tail when full",
    global_rq.tail == PROCESS_MAX
    );

  size_t tail = global_rq.tail;
  _assert_message(
    "Returns error if run_queue is full",
    add_process_rq(&pcb) == ERROR_CODE &&
    tail == global_rq.tail
    );

  pop_earliest_deadline_rq();

  _assert_message(
    "Can again add element after remove one",
    global_rq.tail == tail - 1 &&
    add_process_rq(&pcb) == 0
    );

  sched_bfs_rst();
  return 0;
}

int schedule_process_rq_test() {
  const pid_t pid_test = 10;
  pcb_t pcb = { .pid = pid_test };

  size_t index = global_rq.tail;
  int err = sched_process_rq(&pcb);

  _assert_message(
    "Should get deadline and timeslice",
    !err &&
    global_rq.run_queue[index].pid == pid_test &&
    global_rq.run_queue[index].deadline >= global_rq.jiffies + TIME_SLICE
    );

  sched_bfs_rst();
  return 0;
}

int add_process_rq_order_test() {
  pcb_t pcb1 = { .pid = 1, .deadline = 10};
  pcb_t pcb2 = { .pid = 2, .deadline = 30};
  pcb_t pcb3 = { .pid = 3, .deadline = 5};

  add_process_rq(&pcb1);
  add_process_rq(&pcb2);
  add_process_rq(&pcb3);

  _assert_message(
    "Earliest deadline returns correct entry.",
    pop_earliest_deadline_rq() == 3 &&
    pop_earliest_deadline_rq() == 1 &&
    pop_earliest_deadline_rq() == 2
    );

  return 0;
}


int all_tests_sched_bfs() {
  _verify(add_process_rq_test);
  _verify(add_process_when_full_test);
  _verify(schedule_process_rq_test);
  _verify(add_process_rq_order_test);
  return 0;
}
