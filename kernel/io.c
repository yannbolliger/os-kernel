

#include "io.h"


size_t uart_write(const PL011_t* uart, const char* x, const size_t n) {
  for (int i = 0; i < n; i++) {
    PL011_putc(UART0, *x++, true);
  }
  return n;
}
