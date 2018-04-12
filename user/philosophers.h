
/**
 * Dining philosophers
 * --
 * This program solves the known problem of the "dining philosophers".
 * It uses POSIX unnamed pipes and forks in order to acheive this.
 * As the OS kernel by Yann Bolliger conforms to the minimal subset of POSIX
 * for these commands, the program is compileable and runnable on POSIX as
 * well as on the OS kernel by Yann Bolliger.
 *
 * The only thing you need that changes are the includes below and the linking.
 */


#ifndef __PHILOSOPHERS_H
#define __PHILOSOPHERS_H

/**
 * RUN on OS kernel by Yann Bolliger.
 * Uncomment this line:
 */

#include "libc.h"

/**
 * RUN on POSIX.
 * Uncomment these lines:
 */

 // #include <stdlib.h>
 // #include <stdio.h>
 // #include <unistd.h>

#define PHIL_NUMBER (16)
#define ROUNDS (10)

typedef enum {
  FIRST_FORK,
  SECOND_FORK
} fork_t;


#endif
