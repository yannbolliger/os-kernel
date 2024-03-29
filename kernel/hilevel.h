
#ifndef __HILEVEL_H
#define __HILEVEL_H

// Include functionality relating to newlib (the standard C library).

#include <stdbool.h>
#include <stddef.h>

// Include device definitions
#include "GIC.h"
#include "PL011.h"
#include "SP804.h"
#include "MMU.h"

// Include functionality relating to the   kernel.
#include "lolevel.h"
#include "int.h"
#include "kernel.h"
#include "page.h"
#include "io.h"
#include "sched.h"
#include "pipe.h"


#endif
