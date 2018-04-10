


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
} pipe_t;

int pipe_create(const pid_t pid, int* fd);
int pipe_read(const pipe_t* pipe, char* buf, const size_t n);
int pipe_write(const pipe_t* pipe, const char* buf, const size_t n);
int pipe_close(const pid_t pid, const int fd);

#endif
