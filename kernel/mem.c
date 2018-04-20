

#include "mem.h"

#define BITS_PER_ELEM (sizeof(uint64_t) * 8)
#define MEM_MAP_SIZE  (MEM_BLOCK_NUMBER/BITS_PER_ELEM)

/**
 * Bitmap for allocation of the memory region between MEM_LO and MEM_HI.
 * LSB of mem_map[0] corresponds to memory block at address MEM_LO.
 */
uint64_t mem_map[MEM_MAP_SIZE];

void mem_rst() {
  memset(mem_map, 0, sizeof(uint64_t) * MEM_MAP_SIZE);
}

int find_first_unset(const size_t start_index) {

  for (size_t int_index = start_index; int_index < MEM_MAP_SIZE; ++int_index) {
    uint64_t element = mem_map[int_index];
    int bit_index = ffs(~element);

    if (bit_index > 0) return int_index * BITS_PER_ELEM + bit_index - 1;
  }
  return -1;
}

int test_bit(size_t index) {
  return (mem_map[index >> 6] >> (index & (BITS_PER_ELEM - 1))) & 1UL;
}

void set_bit(size_t index) {
  mem_map[index >> 6] |= (1UL << (index & (BITS_PER_ELEM - 1)));
}

void clear_bit(size_t index) {
  mem_map[index >> 6] &= ~(1UL << (index & (BITS_PER_ELEM - 1)));
}

int test_all(size_t index, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    if (!test_bit(index + i)) return 0;
  }
  return 1;
}

int test_any(size_t index, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    if (test_bit(index + i)) return 1;
  }
  return 0;
}

/**
 * Attempts to allocate n blocks of contiguous memory and returns the lowest
 * address on success. Otherwise it returns 0.
 * @param n_blocks - number of memory blocks to allocate
 */
uint32_t mem_allocate(const size_t n_blocks) {
  if (n_blocks == 0) return 0;

  int index = 0;

  while (test_any(index, n_blocks)) {
    index = find_first_unset(index);
    if (index == -1) return 0;
  }

  for (size_t i = 0; i < n_blocks; i++) {
    set_bit(index + i);
  }
  return index * MEM_BLOCK_SIZE + MEM_LO;
}

size_t mem_deallocate(const uint32_t address, const size_t n_blocks) {
  const size_t index = (address - MEM_LO) / MEM_BLOCK_SIZE;

  if (!test_all(index, n_blocks)) return 0;

  for (size_t i = 0; i < n_blocks; ++i) {
    clear_bit(index + i);
  }
  return n_blocks;
}

size_t mem_copy(const uint32_t src_addr, const uint32_t dst_addr,
    const size_t n_blocks) {

  const size_t src_index = (src_addr - MEM_LO) / MEM_BLOCK_SIZE;
  const size_t dst_index = (dst_addr - MEM_LO) / MEM_BLOCK_SIZE;

  // check that all involved memory is allocated
  if (!test_all(src_index, n_blocks) || !test_all(dst_index, n_blocks))
    return 0;

  // check that regions do not overlap
  if (src_index < dst_index && src_index + n_blocks > dst_index) return 0;
  if (dst_index < src_index && dst_index + n_blocks > src_index) return 0;

  memcpy((void *) dst_addr, (void *) src_addr, n_blocks * MEM_BLOCK_SIZE);
  return n_blocks;
}
