


#include "pipe.h"

typedef struct {
  uint32_t mem_base_addr;
  size_t head;
  size_t tail;
} pipe_t;

pipe_t pipe_table[PIPE_MAX];
size_t pipe_table_tail = 0;


int pipe_create(const pid_t pid, int* fd) {
  if (pipe_table_tail == PIPE_MAX) return ERROR_CODE;

  uint32_t mem_base_addr = mem_allocate(1);
  if (mem_base_addr == 0) return ERROR_CODE;

  pipe_t* pipe = &pipe_table[pipe_table_tail];
  pipe->mem_base_addr = mem_base_addr;
  pipe->head = 0;
  pipe->tail = 0;

  fd_t fd_read  = { R_OK, pipe };
  fd_t fd_write = { W_OK, pipe };

  int fd_int_read  = set_fd(pid, fd_read);
  int fd_int_write = set_fd(pid, fd_write);

  if (fd_int_read < 0 || fd_int_write) {
    mem_deallocate(mem_base_addr, 1);
    return ERROR_CODE;
  }

  // commit action and return
  pipe_table_tail++;
  *fd[0] = fd_int_read;
  *fd[1] = fd_int_write;

  return 0;
}

int pipe_read(const pid_t pid, const int fd, char* buf, const size_t n) {
  return 0;
}

int pipe_write(const pid_t pid, const int fd, const char* buf, const size_t n) {
  return 0;
}

int pipe_close(const pid_t pid, const int fd) {
  return 0;
}
