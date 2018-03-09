
#ifndef __HILEVEL_H
#define __HILEVEL_H

// Include functionality relating to newlib (the standard C library).

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Include device definitions
#include "SP804.h"

// Include functionality relating to the   kernel.
#include "lolevel.h"
#include     "int.h"

// interval for pre-emptive multitasking: 2^20 ticks
#define TIMER_INTERVAL_TICKS (0x00100000);

#endif
