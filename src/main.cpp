#include "bump_arena.hpp"
#include "matrix.hpp"
int main() {
  BumpArena<float> arena(100 * 1024 * 1024 / sizeof(float));
  Matrix A{512, 1024, arena};
  A.fill(1.0f);
  Matrix B{1024, 768, arena};
  B.fill(2.0f);

  Matrix C = A.mul(B, arena);
  C.print();
}
