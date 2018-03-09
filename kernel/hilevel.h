
#ifndef __HILEVEL_H
#define __HILEVEL_H

// Include functionality relating to newlib (the standard C library).

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Include device definitions
#include "GIC.h"
#include "PL011.h"
#include "SP804.h"
#include "libc.h"

// Include functionality relating to the   kernel.
#include "lolevel.h"
#include "int.h"
#include "kernel.h"
#include "scheduler.h"

// interval for pre-emptive multitasking: 2^20 ticks
#define TIMER_INTERVAL_TICKS (0x00100000);



#endif
