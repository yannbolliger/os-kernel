


#include "bitmap.h"

int find_first_unset(const uint64_t* map, const size_t size, const size_t start_index) {

  for (size_t int_index = start_index; int_index < size; ++int_index) {
    uint64_t element = map[int_index];
    int bit_index = ffs(~element);

    if (bit_index > 0) return int_index * BITS_PER_ELEM + bit_index - 1;
  }
  return -1;
}

int test_bit(const uint64_t* map, size_t index) {
  return (map[index >> 6] >> (index & (BITS_PER_ELEM - 1))) & 1UL;
}

void set_bit(uint64_t* map, size_t index) {
  map[index >> 6] |= (1UL << (index & (BITS_PER_ELEM - 1)));
}

void clear_bit(uint64_t* map, size_t index) {
  map[index >> 6] &= ~(1UL << (index & (BITS_PER_ELEM - 1)));
}

int test_all(const uint64_t* map, size_t index, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    if (!test_bit(map, index + i)) return 0;
  }
  return 1;
}

int test_any(const uint64_t* map, size_t index, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    if (test_bit(map, index + i)) return 1;
  }
  return 0;
}
