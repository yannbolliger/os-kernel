
#ifndef __COOL_CONSOLE_H
#define __COOL_CONSOLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "libc.h"

#define PROGRAM_NAME_MAX (32)

/**
 * Import these functions from the original console.c
 */
extern void puts(char* x, int n);
extern void gets(char* x, int n);

#endif
