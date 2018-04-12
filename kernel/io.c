

#include "io.h"


size_t uart_read(PL011_t* uart, char* x, const size_t n) {
  for (int i = 0; i < n; i++) {
    x[i] = PL011_getc(uart, true);

    if (x[i] == '\x0A') {
      x[i] = '\x00';
      return i + 1;
    }
  }
  return n;
}

size_t uart_write(PL011_t* uart, const char* x, const size_t n) {
  for (int i = 0; i < n; i++) {
    PL011_putc(UART0, *x++, true);
  }
  return n;
}

int set_fd(const pid_t pid, const fd_t fd) {
  pcb_t* pcb = pcb_of(pid);

  if (OPEN_MAX == pcb->fd_tail) return ERROR_CODE;

  pcb->fd_table[pcb->fd_tail] = fd;
  return pcb->fd_tail++;
}

fd_t* get_fd(const pid_t pid, const int fd) {
  pcb_t* pcb = pcb_of(pid);
  if (fd < 0 || fd >= OPEN_MAX || pcb == NULL) return NULL;

  return &pcb->fd_table[fd];
}

int io_read(const pid_t pid, const int fd, char* x, const size_t n) {
  fd_t* fildes = get_fd(pid, fd);
  if (fildes == NULL) return ERROR_CODE;

  // check permission to read
  if (STDOUT_FILENO == fd || STDERR_FILENO == fd || fildes->mode & R_OK == 0) {
    return ERROR_CODE;
  }

  if (STDIN_FILENO == fd)
    return uart_read(UART0, x, n);
  else
    return pipe_read(fildes->file, x, n);
}


int io_write(const pid_t pid, const int fd, const char* x, const size_t n) {
  fd_t* fildes = get_fd(pid, fd);
  if (fildes == NULL) return ERROR_CODE;

  // check permission to write
  if (STDIN_FILENO == fd || fildes->mode & W_OK == 0) return ERROR_CODE;

  if (STDOUT_FILENO == fd || STDERR_FILENO == fd)
    return uart_write(UART0, x, n);
  else
    return pipe_write(fildes->file, x, n);
}

int io_close(const pid_t pid, const int fd) {
  fd_t* fildes = get_fd(pid, fd);
  if (fildes == NULL) return ERROR_CODE;

  if (fd < STDERR_FILENO) return ERROR_CODE;

  int err = pipe_close(fildes->file);
  if (err) return ERROR_CODE;

  memset(fildes, 0, sizeof(fd_t));
  return 0;
}
