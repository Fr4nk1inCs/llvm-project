#include <cstddef>

using f64_t = double;

namespace helper {
// NOLINTBEGIN(readability-identifier-naming)
const size_t M = 2048;
const size_t K = 2048;
const size_t N = 2048;
// NOLINTEND(readability-identifier-naming)

const f64_t alpha = 1.0;
const f64_t beta = 1.0;

void initMatrices(f64_t matA[][K], f64_t matB[][N], f64_t matC[][N]) noexcept;
void printMatrix(f64_t *mat, size_t rows, size_t cols) noexcept;
} // namespace helper

extern "C" {
void printFlops(f64_t time) noexcept;
void printTime(f64_t time) noexcept;
f64_t getTime() noexcept;
}
