#pragma once

#include "bump_arena.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>

class Matrix {
public:
  std::size_t rows;
  std::size_t cols;
  float *data;

  Matrix(std::size_t rows, std::size_t cols, BumpArena<float> &arena);

  inline float &operator()(std::size_t i, std::size_t j) {
    return data[i * cols + j];
  }

  inline const float &operator()(std::size_t i, std::size_t j) const {
    return data[i * cols + j];
  }

  void fill(float value);
  void rand(float min_val = -1.0f, float max_val = 1.0f);

  Matrix mul(const Matrix &other, BumpArena<float> &arena) const;

  void print() const;
};
