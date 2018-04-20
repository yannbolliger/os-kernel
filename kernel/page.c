

#include "page.h"

#define PAGE_MAP_SIZE (PAGE_TOTAL_NUMBER/BITS_PER_ELEM)

uint64_t page_map[PAGE_MAP_SIZE];


/* T is a page table, which, for the 1MB pages (or sections) we use,
 * has 4096 entries: note the need to align T to a multiple of 16kB.
 */

pte_t T[4096] __attribute__ ((aligned (1 << 14)));


void page_rst() {
  memset(page_map, 0, sizeof(uint64_t) * PAGE_MAP_SIZE);

  for (int i = 0; i < 0x703; i++) {
    T[i] = ((pte_t)(i) << 20) | 0x00C22;
  }

  // Protect page 700
  // mask access
  T[0x700] &= ~0x08C00;
  // set  access 0b110 => read-only for all
  T[0x700] |=  0x08800;


  // Protect page 701
  // mask access
  T[0x701] &= ~0x08C00;
  // set  access 0b010 => kernel RW, no access for user
  T[0x701] |=  0x00400;

  // General data (LIBC) page 702
  // mask access
  T[0x702] &= ~0x08C00;
  // set  access 0b011 => RW for all
  T[0x702] |=  0x00C00;

  for (int i = 0x703; i < 4096; i++) {
    T[i] = ((pte_t)(i) << 20) | 0x00422;
  }

  // Virtual stack page 0xFFF
  // set  access 0b011 => RW for all
  T[0xFFF] =  0x00000C22;

  // configure and enable MMU
  mmu_set_ptr0(T);

  // set domain 0 to 11_{(2)} => manager (i.e., not checked)
  mmu_set_dom(0, 0x3);

  // set domain 1 to 01_{(2)} => client  (i.e.,     checked)
  mmu_set_dom(1, 0x1);

  mmu_enable();
}

void page_swap_stack(pcb_t* pcb) {
  if (NULL == pcb) return;

  // Virtual stack page 0xFFF
  // mask physical page
  T[0xFFF] &= ~0xFFF00000;
  // set physical page
  T[0xFFF] |=  pcb->stack_page << 20;

  mmu_flush();
}

size_t page_allocate() {
  int index = find_first_unset(page_map, PAGE_MAP_SIZE, 0);
  if (index == -1) return 0;

  set_bit(page_map, index);
  return index + PAGE_LO;
}

int page_deallocate(const size_t page) {
  const size_t index = (page - PAGE_LO);

  if (!test_bit(page_map, index)) return ERROR_CODE;

  clear_bit(page_map, index);
  return 0;
}

int page_copy(const size_t src_page, const size_t dst_page) {

  const size_t src_index = (src_page - PAGE_LO);
  const size_t dst_index = (dst_page - PAGE_LO);

  // check that all involved memory is allocated
  if (!test_bit(page_map, src_index) ||
      !test_bit(page_map, dst_index) ||
      src_index == dst_index)
    return ERROR_CODE;

  memcpy((void *) (dst_page*PAGE_SIZE), (void *) (src_page*PAGE_SIZE), PAGE_SIZE);
  return 0;
}
