

#include "io.h"



int set_fd(const pid_t pid, const fd_t fd) {
  pcb_t* pcb = pcb_of(pid);

  if (OPEN_MAX == pcb->fd_tail) return ERROR_CODE;

  pcb->fd_table[pcb->fd_tail] = fd;
  return pcb->fd_tail++;
}

int io_write(const pid_t pid, const int fd, const char* x, const size_t n) {
  // check existence
  pcb_t* pcb = pcb_of(pid);
  if (fd < 0 || fd >= OPEN_MAX || pcb == NULL) return ERROR_CODE;

  // check permission to write
  fd_t* fildes = &pcb->fd_table[fd];
  if (STDIN_FILENO == fd || fildes->mode & W_OK == 0) return ERROR_CODE;

  if (STDOUT_FILENO == fd || STDERR_FILENO == fd)
    return uart_write(UART0, x, n);
  else
    return pipe_write(fildes->file, x, n);
}


size_t uart_write(const PL011_t* uart, const char* x, const size_t n) {
  for (int i = 0; i < n; i++) {
    PL011_putc(UART0, *x++, true);
  }
  return n;
}
