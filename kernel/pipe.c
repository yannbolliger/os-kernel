

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

  if (fd_int_read < 0 || fd_int_write < 0) {
    mem_deallocate(mem_base_addr, 1);
    return ERROR_CODE;
  }

  // commit action and return
  pipe_table_tail++;
  fd[0] = fd_int_read;
  fd[1] = fd_int_write;

  return 0;
}

int pipe_read(pipe_t* pipe, void* buf, const size_t n) {
  if (NULL == pipe) return ERROR_CODE;

  size_t n_to_read = (n > pipe->length) ? pipe->length : n;

  if (pipe->head + n_to_read >= MEM_BLOCK_SIZE) {
    const size_t n_from_0 = (pipe->head + n_to_read) % MEM_BLOCK_SIZE;

    memcpy(buf, (void*) (pipe->mem_base_addr + pipe->head), n_to_read - n_from_0);
    memcpy(buf + (n_to_read - n_from_0), (void*) (pipe->mem_base_addr), n_from_0);
    pipe->head = n_from_0;
  }
  else {
    memcpy(buf, (void*) (pipe->mem_base_addr + pipe->head), n_to_read);
    pipe->head += n_to_read;
  }

  pipe->length -= n_to_read;
  return n_to_read;
}

int pipe_write(pipe_t* pipe, const void* buf, const size_t n) {
  if (NULL == pipe) return ERROR_CODE;

  size_t n_to_write = n;
  if (pipe->length + n > MEM_BLOCK_SIZE) {
    n_to_write = MEM_BLOCK_SIZE - pipe->length;
  }

  size_t index = pipe->head + pipe->length;
  if (index + n_to_write >= MEM_BLOCK_SIZE) {
    const size_t n_from_0 = (index + n_to_write) % MEM_BLOCK_SIZE;

    memcpy((void*) (pipe->mem_base_addr + index), buf, n_to_write - n_from_0);
    memcpy((void*) (pipe->mem_base_addr), buf + (n_to_write - n_from_0), n_from_0);
  }
  else {
    memcpy((void*) (pipe->mem_base_addr + index), buf, n_to_write);
  }

  pipe->length += n_to_write;
  return n_to_write;
}

int pipe_close(const pid_t pid, const int fd) {
  return 0;
}
