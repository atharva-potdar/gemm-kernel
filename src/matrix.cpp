#include "matrix.hpp"
#include <random>

Matrix::Matrix(std::size_t rows, std::size_t cols, BumpArena<float> &arena)
    : rows(rows), cols(cols) {
  data = arena.allocate(rows * cols);
}

void Matrix::fill(float value) { std::fill(data, data + (rows * cols), value); }

void Matrix::rand(float min_val, float max_val) {
  // thread_local ensures the RNG is only seeded once
  thread_local std::random_device rd;
  thread_local std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min_val, max_val);

  for (std::size_t i = 0; i < rows * cols; i++) {
    data[i] = dis(gen);
  }
}

// Assume multiplication is valid and possible
Matrix Matrix::mul(const Matrix &other, BumpArena<float> &arena) const {
  Matrix result(this->rows, other.cols, arena);

  std::size_t block_size = 32;
  std::size_t p = this->rows;
  std::size_t q = this->cols;
  std::size_t r = other.cols;

#pragma omp parallel for schedule(static)
  for (std::size_t i_block = 0; i_block < p; i_block += block_size) {
    std::size_t i_max = std::min(i_block + block_size, p);
    for (std::size_t j_block = 0; j_block < r; j_block += block_size) {
      std::size_t j_max = std::min(j_block + block_size, r);
      for (std::size_t i = i_block; i < i_max; i++)
        for (std::size_t j = j_block; j < j_max; j++)
          result.data[i * r + j] = 0.0f;
      for (std::size_t k_block = 0; k_block < q; k_block += block_size) {
        std::size_t k_max = std::min(k_block + block_size, q);
        for (std::size_t i = i_block; i < i_max; i++) {
          for (std::size_t k = k_block; k < k_max; k++) {
            float a_ik = this->data[(i * q) + k]; // remains the same
            for (std::size_t j = j_block; j < j_max; j++) {
              result.data[(i * r) + j] += a_ik * other.data[j + (k * r)];
            }
          }
        }
      }
    }
  }

  return result;
}

void Matrix::print() const {
  for (std::size_t i = 0; i < this->rows * this->cols; i++) {
    std::cout << this->data[i];
    std::cout << (((i + 1) % this->cols == 0) ? "\n" : " ");
  }
}
