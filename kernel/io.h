

#ifndef __IO_H
#define __IO_H

#include <string.h>

#include "kernel.h"
#include "PL011.h"


int get_fd(const pid_t pid);

size_t uart_write(const PL011_t* uart, const char* x, const size_t n);

#endif
