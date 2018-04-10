

#include "io.h"



int set_fd(const pid_t pid, const fd_t fd) {
  pcb_t* pcb = pcb_of(pid);

  if (OPEN_MAX == pcb->fd_tail) return ERROR_CODE;

  pcb->fd_table[pcb->fd_tail] = fd;
  return pcb->fd_tail++;
}


size_t uart_write(const PL011_t* uart, const char* x, const size_t n) {
  for (int i = 0; i < n; i++) {
    PL011_putc(UART0, *x++, true);
  }
  return n;
}
