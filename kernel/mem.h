
/**
 * @file mem.h
 * @brief This module manages the memory that belongs to the kernel.
 *
 * @date April 2018
 * @author Yann Bolliger
 */

/**
 * MEMORY
 *
 * Because we don't have a heap, the kernel needs to manage memory for tables,
 * pipes etc. itself.
 * The facilitate the task, this module can allocate/reserve memory blocks of
 * size MEM_BLOCK_SIZE (4069B). It uses a bitmap to keep track of free blocks.
 */

#ifndef __MEM_H
#define __MEM_H

#include <string.h>
#include "kernel.h"
#include "bitmap.h"

/**
 * Memory boundaries
 *
 * These are the limits of the available memory inside the kernel memory page.
 * They stem from the definitions in the linker script.
 * (ALSO UPDATE THESE IN THE LINKER SCRIPT!)
 */
#define MEM_LO 0x701A0000
#define MEM_HI 0x70200000

/**
 * The smallest unit of memory that is managed.
 * Contigous blocks can be allocated or deallocated.
 */
#define MEM_BLOCK_SIZE   (0x1000)
#define MEM_BLOCK_NUMBER ((MEM_HI - MEM_LO)/(MEM_BLOCK_SIZE))

/**
 * INTERFACE
 */


void mem_rst();

uint32_t mem_allocate(const size_t n_blocks);

size_t   mem_deallocate(const uint32_t address, const size_t n_blocks);

size_t   mem_copy(const uint32_t src_addr, const uint32_t dst_addr,
    const size_t n_blocks);

static inline uint32_t mem_block_addr_end(uint32_t addr_start) {
  return addr_start + MEM_BLOCK_SIZE - 1;
}

#endif
