

#ifndef __IO_H
#define __IO_H

#include <string.h>

#include "kernel.h"
#include "pipe.h"
#include "process.h"
#include "PL011.h"


int set_fd(const pid_t pid, const fd_t fd);
int io_write(const pid_t pid, const int fd, const char* x, const size_t n);

size_t uart_write(const PL011_t* uart, const char* x, const size_t n);

#endif
