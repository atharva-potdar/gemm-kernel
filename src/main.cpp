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

  for (std::size_t i = 0; i < p; i++) {
    for (std::size_t j = 0; j < r; j++) {
      float sum = 0;
      for (std::size_t k = 0; k < q; k++) {
        sum += A[(i * q) + k] * B[j + (k * r)];
      }
      C[i * r + j] = sum;
    }
  }

  for (std::size_t i = 0; i < p * r; i++) {
    std::cout << C[i];
    std::cout << (((i + 1) % r == 0) ? "\n" : " ");
  }
  std::cout << std::endl;
}
