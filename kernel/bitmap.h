
/**
 * @file bitmap.h
 * @brief This module provides a clean (use-case agnostic) bitmap interface.
 *
 * @date April 2018
 * @author Yann Bolliger
 */

#ifndef __BITMAP_H
#define __BITMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

#define BITS_PER_ELEM (sizeof(uint64_t) * 8)

/**
 * INTERFACE
 */



 int find_first_unset(const uint64_t* map, const size_t size, const size_t start_index);

 int test_bit(const uint64_t* map, size_t index);
 void set_bit(uint64_t* map, size_t index);
 void clear_bit(uint64_t* map, size_t index);
 int test_all(const uint64_t* map, size_t index, size_t n);
 int test_any(const uint64_t* map, size_t index, size_t n);

#endif
