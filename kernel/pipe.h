


#ifndef __PIPE_H
#define __PIPE_H

#include "kernel.h"
#include "io.h"
#include "mem.h"


#define PIPE_MAX (PROCESS_MAX * OPEN_MAX)

typedef struct {
  uint32_t mem_base_addr;
  size_t head;
  size_t length;
  size_t opened_fds;
} pipe_t;

int pipe_create(const pid_t pid, int* fd);
int pipe_read(pipe_t* pipe, void* buf, const size_t n);
int pipe_write(pipe_t* pipe, const void* buf, const size_t n);
int pipe_close(const pid_t pid, const int fd);

#endif
