


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



int round_robin_test() {

  // setup test case
  ctx_t ctx = {0};
  scheduler_rst(&ctx);

  scheduler(&ctx);

  _assert_message(
    "Should execute second process",
    pcb_table.executing_pid == 1);

  return 0;
}


int all_tests_scheduler() {
  _verify(round_robin_test);
  return 0;
}
