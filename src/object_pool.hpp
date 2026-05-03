#pragma once
#include <cassert>
#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>

template <typename T, std::size_t Capacity> class Pool {
private:
  static_assert(sizeof(T) >= sizeof(T *), "T must be at least pointer-sized.");
  static_assert(Capacity > 0, "Capacity must be strictly positive.");
  static_assert(std::is_trivially_destructible_v<T>,
                "Pool does not auto-destruct active elements on shutdown. T "
                "must be trivially destructible.");

  std::unique_ptr<std::byte[]> storage;
  T *free_head;
  size_t allocated;

public:
  Pool() : allocated(0) {
    storage = std::make_unique<std::byte[]>(Capacity * sizeof(T));

    for (std::size_t i = 0; i < Capacity; i++) {
      T *slot = reinterpret_cast<T *>(storage.get() + (i * sizeof(T)));
      T *next =
          (i + 1 < Capacity)
              ? reinterpret_cast<T *>(storage.get() + ((i + 1) * sizeof(T)))
              : nullptr;

      std::memcpy(static_cast<void *>(slot), &next, sizeof(T *));
    }

    free_head = reinterpret_cast<T *>(storage.get());
  }

  template <typename... Args>
  [[nodiscard]] auto allocate(Args &&...args) -> T * {
    if (free_head == nullptr) {
      return nullptr;
    }
    T *result = free_head;
    std::memcpy(&free_head, static_cast<void *>(result), sizeof(T *));
    allocated++;
    return new (result) T(std::forward<Args>(args)...);
  }

  void deallocate(T *ptr) {
    if (ptr == nullptr) {
      return;
    }

    [[maybe_unused]] auto *byte_ptr = reinterpret_cast<std::byte *>(ptr);
    assert(byte_ptr >= storage.get() &&
           byte_ptr < storage.get() + (Capacity * sizeof(T)) &&
           "Pointer does not belong to this Pool");
    assert(static_cast<size_t>(byte_ptr - storage.get()) % sizeof(T) == 0 &&
           "Pointer is not aligned to Pool boundaries");
    assert(allocated > 0 && "Pool allocated count underflow");

    ptr->~T();
    std::memcpy(static_cast<void *>(ptr), &free_head, sizeof(T *));
    free_head = ptr;
    allocated--;
  }

  [[nodiscard]] auto size() const -> size_t { return allocated; }

  [[nodiscard]] auto capacity() const -> size_t { return Capacity; }
};
