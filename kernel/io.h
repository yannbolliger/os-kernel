
/**
 * @file io.h
 * @brief This module is a generic interface to input and output.
 * It handles all the file descriptors of a process.
 *
 * @date April 2018
 * @author Yann Bolliger
 */

/**
 * IO
 *
 * The input/output of this system is limited to UART and pipes.
 * If not otherwise specified, all std[in|out|err] are mapped to UART0.
 */

#ifndef __IO_H
#define __IO_H

#include <string.h>

#include "kernel.h"
#include "pipe.h"
#include "process.h"

#ifndef __TEST
#include "PL011.h"
#endif

#define STD_UART (UART0)

/**
 * INTERFACE
 */

int set_fd(const pid_t pid, const fd_t fd);
int io_read(const pid_t pid, const int fd, char* x, const size_t n);
int io_write(const pid_t pid, const int fd, const char* x, const size_t n);
int io_close(const pid_t pid, const int fd);
void kernel_write_error(const char* x, const size_t n);

#endif
