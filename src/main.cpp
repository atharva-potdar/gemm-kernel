#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
  // Let matrix A be p x q
  // Let matrix B be q x r

  // All matrices will be flat

  std::size_t p = 512;
  std::size_t q = 1024;
  std::size_t r = 768;

  std::vector<float> A(p * q, 1.0f);
  std::vector<float> B(q * r, 2.0f);
  std::vector<float> C(p * r, 0.0f);

  std::size_t block_size = 16;

  for (std::size_t i_block = 0; i_block < p; i_block += block_size) {
    std::size_t i_max = std::min(i_block + block_size, p);
    for (std::size_t k_block = 0; k_block < q; k_block += block_size) {
      std::size_t k_max = std::min(k_block + block_size, q);
      for (std::size_t j_block = 0; j_block < r; j_block += block_size) {
        std::size_t j_max = std::min(j_block + block_size, r);
        for (std::size_t i = i_block; i < i_max; i++) {
          for (std::size_t k = k_block; k < k_max; k++) {
            float a_ik = A[(i * q) + k]; // remains the same
            for (std::size_t j = j_block; j < j_max; j++) {
              C[(i * r) + j] += a_ik * B[j + (k * r)];
            }
          }
        }
      }
    }
  }

  for (std::size_t i = 0; i < p * r; i++) {
    std::cout << C[i];
    std::cout << (((i + 1) % r == 0) ? "\n" : " ");
  }
  std::cout << std::endl;
}
