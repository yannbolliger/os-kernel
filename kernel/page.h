
/**
 * @file page.h
 * @brief This module manages the page table and allocation of the memory that
 * does not belong to the kernel.
 *
 * @date April 2018
 * @author Yann Bolliger
 */

/**
 * VIRTUAL MEMORY
 *
 * This kernel uses the MMU to protect various regions of memory and to provide
 * a uniform stack address space to all user processes. The following memory
 * page layout is used and can be seen in the linker script (image.ld) as
 * well as in the page_rst function where it is configured.
 *
 * Each user process only gets one page of memory for its stack. On rescheduling
 * it suffices therefore to change the mapping of this single virtual stack page.
 * All the other pages are not remapped by the MMU, they only use the access
 * checks of it, not the remapping.
 *
 * Virtual memory pages => Physical memory pages (as given by QEMU):
 *
 *
 * Page 0x700 => 0x700: TEXT PAGE
 * All programs (including the kernel) are stored in this page. It is therefore
 * set to read-only for everyone.
 *
 * Page 0x701 => 0x701: KERNEL MEMORY
 * This pages contains all the data/bss segments of the kernel, in addition
 * it contains the stacks of all the exception handler modes.
 * The rest of the memory is dynamically managed by the mem module.
 * This page can only be accessed by the kernel.
 *
 * Page 0x702 => 0x702: LIBC MEMORY
 * This page contains all user (and libc) data/bss segments. Most user programs
 * don't really use this. But because user programs need libc, they need to be
 * able to write here too.
 * This page is read/write for everyone.
 *
 * Pages 0xFFF => [any of 0x703 - 0x8FF]: USER STACK PAGES
 * From here until the end of the physical memory, all the physical pages are
 * used for stacks of user processes. They are accessed by the users
 * via the remapping e.g. uniformly with number 0xFFF.
 * However the kernel also has the identity mappings at his disposal.
 * It needs this to manage the pages. But these mappings are set to kernel-only
 * access restrictions.
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
