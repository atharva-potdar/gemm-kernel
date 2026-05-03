#pragma once
#include <cassert>
#include <cstddef>
#include <memory>

template <typename T> class BumpArena {
private:
  std::unique_ptr<T[]> memory;
  std::size_t capacity;
  std::size_t offset;

public:
  explicit BumpArena(std::size_t cap) {
    capacity = cap;
    offset = 0;
    memory = std::make_unique<T[]>(capacity);
  }

  // Allocates contiguous array of N elements
  [[nodiscard]] T *allocate(std::size_t n) {
    assert(offset + n <= capacity);
    T *ptr = memory.get() + offset;
    offset += n;
    return ptr;
  }

  void reset() noexcept { offset = 0; }
};
