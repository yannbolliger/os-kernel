

#ifndef __IO_H
#define __IO_H

#include <string.h>

#include "kernel.h"
#include "pipe.h"
#include "process.h"

#ifndef __TEST
#include "PL011.h"
#endif


int set_fd(const pid_t pid, const fd_t fd);
int io_read(const pid_t pid, const int fd, char* x, const size_t n);
int io_write(const pid_t pid, const int fd, const char* x, const size_t n);
int io_close(const pid_t pid, const int fd);
void kernel_write_error(const char* x, const size_t n);

#endif
