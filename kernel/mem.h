

#ifndef __MEM_H
#define __MEM_H

#include <string.h>
#include "kernel.h"

/**
 * Memory boundaries (MUST ALSO UPDATE LINKER SCRIPT!)
 */
#define MEM_LO 0x70020000
#define MEM_HI 0x90000000

#define MEM_BLOCK_SIZE (0x1000)
#define MEM_BLOCK_NUMBER ((MEM_HI - MEM_LO)/(MEM_BLOCK_SIZE))

uint32_t mem_allocate(const size_t n_blocks);

size_t mem_deallocate(const uint32_t address, const size_t n_blocks);

size_t mem_copy(const uint32_t src_addr, const uint32_t dst_addr,
    const size_t n_blocks);

 #endif