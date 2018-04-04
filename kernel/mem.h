

#ifndef __MEM_H
#define __MEM_H

#include "kernel.h"

extern uint32_t MEM_LO;
extern uint32_t MEM_HI;

#define MEM_BLOCK_SIZE (0x1000)

uint32_t mem_allocate(const size_t n_blocks);

size_t mem_deallocate(const uint32_t address, const size_t n_blocks);

size_t mem_copy(const uint32_t src_addr, const uint32_t dst_addr,
    const size_t n_blocks);

#endif
