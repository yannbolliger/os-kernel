


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



int all_tests_sched() {
  return 0;
}
