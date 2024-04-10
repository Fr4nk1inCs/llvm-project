#include "helper.hpp"

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>

void printFlops(f64_t time) noexcept {
  using namespace helper;
  size_t operations = 2 * M * N * K;
  f64_t flops = operations / time;
  std::cout << std::setprecision(2) << flops / 1e9 << " GFLOPS" << std::endl;
}

void printTime(f64_t time) noexcept {
  std::cout << "Compilation Time: " << std::setprecision(6) << time << "s"
            << std::endl;
}

f64_t getTime() noexcept {
  using namespace std::chrono;
  auto systemTime = system_clock::now().time_since_epoch();
  duration<f64_t> f64Sec = systemTime;
  return f64Sec.count();
}

void helper::initMatrices(f64_t matA[][K], f64_t matB[][N],
                          f64_t matC[][N]) noexcept {
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < K; j++) {
      matA[i][j] = i + j;
    }
  }
  for (size_t i = 0; i < K; i++) {
    for (size_t j = 0; j < N; j++) {
      matB[i][j] = i + j;
    }
  }
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      matC[i][j] = 0;
    }
  }
}

void helper::printMatrix(f64_t *mat, size_t rows, size_t cols) noexcept {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < rows; j++) {
      std::cerr << mat[i * cols + j] << " ";
      if (j % 80 == 79)
        std::cerr << std::endl;
    }
    std::cerr << std::endl;
  }
}
