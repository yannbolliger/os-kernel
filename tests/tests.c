
/* runner.c */

#include "min_unit.h"

int tests_run = 0;

int all_tests() {
  _verify(square_01);
  return 0;
}

int main(int argc, char **argv) {
  int result = all_tests();

  if (result == 0) printf("PASSED\n");
  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
