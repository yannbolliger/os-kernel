
/**
 * @file pipe.h
 * @brief Manages all pipes, writes/reads to and from pipes. It is an interface
 * to mainly io but can also directly open pipes (cf. system call pipe).
 *
 * @date April 2018
 * @author Yann Bolliger
 */

#ifndef __PIPE_H
#define __PIPE_H

#include "kernel.h"
#include "io.h"
#include "mem.h"

/**
 * Maximum number of simultaneously open pipes in the system.
 */
#define PIPE_MAX (PROCESS_MAX * (OPEN_MAX/2))

typedef struct {
  uint32_t mem_base_addr;
  size_t head;
  size_t length;
  size_t opened_fds;
} pipe_t;

/**
 * INTERFACE
 */

void pipe_rst();
int pipe_create(const pid_t pid, int* fd);
void pipe_fork(pipe_t* pipe);
int pipe_read(pipe_t* pipe, void* buf, const size_t n);
int pipe_write(pipe_t* pipe, const void* buf, const size_t n);
int pipe_close(pipe_t* pipe);

#endif
