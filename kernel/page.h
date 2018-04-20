
/**
 * @file page.h
 * @brief This module manages the page table and allocation of the memory that
 * does not belong to the kernel.
 *
 * @date April 2018
 * @author Yann Bolliger
 */

#ifndef __PAGE_H
#define __PAGE_H

#include <string.h>

#ifndef __TEST
#include "MMU.h"
#endif

#include "kernel.h"
#include "bitmap.h"

/**
 * Page boundaries
 *
 * (ALSO UPDATE THESE IN THE LINKER SCRIPT!)
 */
#define PAGE_LO (0x703)
#define PAGE_HI (0x900)

#define PAGE_SIZE_EXP     (20)
#define PAGE_SIZE         (1 << PAGE_SIZE_EXP)
#define PAGE_TOTAL_NUMBER (PAGE_HI - PAGE_LO)

#define STACK_PAGE_USER (0xFFF)

typedef uint32_t pte_t;

/**
 * INTERFACE
 */


void page_rst();

void page_swap_stack(pcb_t* next_running_pcb);

static inline uint32_t page_addr_end(uint32_t addr_start) {
  return addr_start + PAGE_SIZE - 1;
}

size_t page_allocate();

int page_deallocate(const size_t page);

int page_copy(const size_t src_page, const size_t dst_page);

#endif
