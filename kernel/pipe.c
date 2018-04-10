


#include "pipe.h"



pipe_t pipe_table[PIPE_MAX];
size_t pipe_table_tail = 0;


int pipe_create(const pid_t pid, int* fd) {
  if (pipe_table_tail == PIPE_MAX) return ERROR_CODE;

  uint32_t mem_base_addr = mem_allocate(1);
  if (mem_base_addr == 0) return ERROR_CODE;

  pipe_t* pipe = &pipe_table[pipe_table_tail];
  pipe->mem_base_addr = mem_base_addr;
  pipe->head = 0;
  pipe->length = 0;

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
  fd[0] = fd_int_read;
  fd[1] = fd_int_write;

  return 0;
}

int pipe_read(pipe_t* pipe, const char* buf, const size_t n) {
  return 0;
}

int pipe_write(pipe_t* pipe, const char* buf, const size_t n) {
  if (NULL == pipe) return ERROR_CODE;

  size_t n_to_write = n;
  if (pipe->length + n > MEM_BLOCK_SIZE) {
    n_to_write = MEM_BLOCK_SIZE - pipe->length;
  }

  if (pipe->head + n_to_write >= MEM_BLOCK_SIZE) {
    const size_t n_from_0 = (pipe->head + n_to_write) % MEM_BLOCK_SIZE;

    memcpy((char *) (pipe->mem_base_addr + pipe->head), buf, n_to_write - n_from_0);
    memcpy((char *) (pipe->mem_base_addr), buf, n_from_0);
    pipe->head = n_from_0;
  }
  else {
    memcpy((char *) (pipe->mem_base_addr + pipe->head), buf, n_to_write);
    pipe->head += n_to_write;
  }

  pipe->length += n_to_write;
  return n_to_write;
}

int pipe_close(const pid_t pid, const int fd) {
  return 0;
}
