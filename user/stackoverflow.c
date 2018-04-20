
#include <string.h>

#include "libc.h"

int infinite_recursion_with_big_stackframe() {
  int huge_array[1 << 10] = {0};

  memset(huge_array, 77, 1 << 10);

  // RECURSE WITHOUT BOUND !!!
  return infinite_recursion_with_big_stackframe();
}

void main_stackoverflow() {
  infinite_recursion_with_big_stackframe();
}
