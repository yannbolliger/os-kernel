


#ifndef __PIPE_H
#define __PIPE_H

#include "kernel.h"
#include "io.h"
#include "mem.h"


#define PIPE_MAX (PROCESS_MAX * (OPEN_MAX - 3))


int pipe_create(const pid_t pid, int* fd);
int pipe_read(const pid_t pid, const int fd, char* buf, const size_t n);
int pipe_write(const pid_t pid, const int fd, const char* buf, const size_t n);
int pipe_close(const pid_t pid, const int fd);

#endif
