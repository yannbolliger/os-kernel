

#ifndef __IO_H
#define __IO_H

#include <string.h>

#include "kernel.h"
#include "PL011.h"


#define PIPES_FILENO_LO (STDERR_FILENO + 10)


size_t uart_write(const PL011_t* uart, const size_t n);

#endif
