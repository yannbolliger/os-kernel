

#include "mem.h"



#define MEM_MAP_SIZE  (MEM_BLOCK_NUMBER/BITS_PER_ELEM)

#define PAGE_MAP_SIZE (PAGE_TOTAL_NUMBER/BITS_PER_ELEM + 1)

/**
 * Bitmap for allocation of the memory region between MEM_LO and MEM_HI.
 * LSB of mem_map[0] corresponds to memory block at address MEM_LO.
 */
uint64_t mem_map[MEM_MAP_SIZE];
uint64_t page_map[PAGE_MAP_SIZE];

void mem_rst() {
  memset(mem_map, 0, sizeof(uint64_t) * MEM_MAP_SIZE);
  memset(page_map, 0, sizeof(uint64_t) * MEM_MAP_SIZE);
}

/**
 * Attempts to allocate n blocks of contiguous memory and returns the lowest
 * address on success. Otherwise it returns 0.
 * @param n_blocks - number of memory blocks to allocate
 */
uint32_t mem_allocate(const size_t n_blocks) {
  if (n_blocks == 0) return 0;

  int index = 0;

  while (test_any(mem_map, index, n_blocks)) {
    index = find_first_unset(mem_map, MEM_MAP_SIZE, index);
    if (index == -1) return 0;
  }

  for (size_t i = 0; i < n_blocks; i++) {
    set_bit(mem_map, index + i);
  }
  return index * MEM_BLOCK_SIZE + MEM_LO;
}

size_t mem_deallocate(const uint32_t address, const size_t n_blocks) {
  const size_t index = (address - MEM_LO) / MEM_BLOCK_SIZE;

  if (!test_all(mem_map, index, n_blocks)) return 0;

  for (size_t i = 0; i < n_blocks; ++i) {
    clear_bit(mem_map, index + i);
  }
  return n_blocks;
}

size_t mem_copy(const uint32_t src_addr, const uint32_t dst_addr,
    const size_t n_blocks) {

  const size_t src_index = (src_addr - MEM_LO) / MEM_BLOCK_SIZE;
  const size_t dst_index = (dst_addr - MEM_LO) / MEM_BLOCK_SIZE;

  // check that all involved memory is allocated
  if (!test_all(mem_map, src_index, n_blocks) || !test_all(mem_map, dst_index, n_blocks))
    return 0;

  // check that regions do not overlap
  if (src_index < dst_index && src_index + n_blocks > dst_index) return 0;
  if (dst_index < src_index && dst_index + n_blocks > src_index) return 0;

  memcpy((void *) dst_addr, (void *) src_addr, n_blocks * MEM_BLOCK_SIZE);
  return n_blocks;
}
