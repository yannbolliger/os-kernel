

#include "min_unit.h"
#include "../kernel/process.h"


extern pcb_table_t pcb_table;
ctx_t ctx = {0};

/**
 * Helpers
 */

void dirty_table_reset() {
  pcb_table.tail = 0;
  pcb_table.executing_pcb = NULL;
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

  dirty_table_reset();
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

  dirty_table_reset();
  return 0;
}

int create_process_full_test() {
  _assert_message(
    "Create process returns valid pid",
    0 != create_process(0, 0)
    );

  for (int i = 0; i < MAX_NUMBER_PROCESSES - 2; i++) {
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

  dirty_table_reset();
  return 0;
}

int all_process_tests() {
  _verify(pcb_of_test);
  _verify(executing_process_test);
  _verify(create_process_full_test);
  return 0;
}
