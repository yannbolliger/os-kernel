
/* runner.c */

#include "min_unit.h"

int tests_run = 0;
extern int all_tests_sched_rq();
extern int all_tests_sched();

int all_tests() {
  return all_tests_sched() + all_tests_sched_rq();
}

int main(int argc, char **argv) {
  int result = all_tests();

  if (result == 0) printf("PASSED\n");
  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
