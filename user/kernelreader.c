
#include <string.h>

#include "libc.h"

#define LENGTH (100)

void main_kernelreader() {
  const uint32_t* kernel_memory = (uint32_t *) 0x70100000;
  char buffer[LENGTH] = {0};

  // try to read some data from kernel memory page
  memcpy(&buffer, kernel_memory, LENGTH);

  return;
}
