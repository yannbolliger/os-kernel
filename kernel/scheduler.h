

#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "kernel.h"

void scheduler_rst(ctx_t* ctx);

void scheduler(ctx_t* ctx);

#endif
