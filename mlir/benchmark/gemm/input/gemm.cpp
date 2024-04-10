#include "helper.hpp"
#include <fstream>

f64_t matA[helper::M][helper::K];
f64_t matB[helper::K][helper::N];
f64_t matC[helper::M][helper::N];

#ifdef BLIS
#include <blis/blis.h>
#elif defined(OPENBLAS)
#include <cblas.h>
#elif defined(MKL)
#include <mkl.h>
#elif defined(TILED)
#ifdef COPY
void tiledGemm(size_t mb, size_t nb, size_t kb) noexcept {
  using namespace helper;
  for (size_t i = 0; i < M; i += mb) {
    for (size_t j = 0; j < N; j += nb) {
      f64_t tileC[mb][nb];
      for (size_t ii = 0; ii < mb; ii++) {
        for (size_t jj = 0; jj < nb; jj++) {
          tileC[ii][jj] = matC[i + ii][j + jj];
        }
      }
      for (size_t k = 0; k < K; k += kb) {
        f64_t tileA[mb][kb];
        f64_t tileB[kb][nb];
        for (size_t ii = 0; ii < mb; ii++) {
          for (size_t kk = 0; kk < kb; kk++) {
            tileA[ii][kk] = matA[i + ii][k + kk];
          }
        }
        for (size_t kk = 0; kk < kb; kk++) {
          for (size_t jj = 0; jj < nb; jj++) {
            tileB[kk][jj] = matB[k + kk][j + jj];
          }
        }
        for (size_t ii = 0; ii < mb; ii++) {
          for (size_t jj = 0; jj < nb; jj++) {
            for (size_t kk = 0; kk < kb; kk++) {
              tileC[ii][jj] =
                  alpha * tileA[ii][kk] * tileB[kk][jj] + beta * tileC[ii][jj];
            }
          }
        }
      }
      for (size_t ii = 0; ii < mb; ii++) {
        for (size_t jj = 0; jj < nb; jj++) {
          matC[i + ii][j + jj] = tileC[ii][jj];
        }
      }
    }
  }
}
#else
void tiledGemm(size_t mb, size_t nb, size_t kb) noexcept {
  using namespace helper;
  for (size_t i = 0; i < M; i += mb) {
    for (size_t j = 0; j < N; j += nb) {
      for (size_t k = 0; k < K; k += kb) {
        for (size_t ii = 0; ii < mb; ii++) {
          for (size_t jj = 0; jj < nb; jj++) {
            for (size_t kk = 0; kk < kb; kk++) {
              matC[i + ii][j + jj] =
                  alpha * matA[i + ii][k + kk] * matB[k + kk][j + jj] +
                  beta * matC[i + ii][j + jj];
            }
          }
        }
      }
    }
  }
}
#endif
#elif defined(IKL)
void vanillaGemm() noexcept {
  using namespace helper;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < K; j++) {
      for (size_t k = 0; k < N; k++) {
        matC[i][k] = alpha * matA[i][j] * matB[j][k] + beta * matC[i][k];
      }
    }
  }
}
#else
void vanillaGemm() noexcept {
  using namespace helper;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      for (size_t k = 0; k < K; k++) {
        matC[i][j] = alpha * matA[i][k] * matB[k][j] + beta * matC[i][j];
      }
    }
  }
}
#endif

int main() {
  helper::initMatrices(matA, matB, matC);

  f64_t alpha = helper::alpha;
  f64_t beta = helper::beta;

  f64_t tStart = getTime();

#ifdef BLIS
  bli_dgemm(BLIS_NO_TRANSPOSE, BLIS_NO_TRANSPOSE, helper::M, helper::N,
            helper::K, &alpha, &matA[0][0], helper::K, 1, &matB[0][0],
            helper::N, 1, &beta, &matC[0][0], helper::N, 1);
#elif defined(OPENBLAS)
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, helper::M, helper::N,
              helper::K, alpha, &matA[0][0], helper::K, &matB[0][0], helper::N,
              beta, &matC[0][0], helper::N);
#elif defined(MKL)
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, helper::M, helper::N,
              helper::K, alpha, &matA[0][0], helper::K, &matB[0][0], helper::N,
              beta, &matC[0][0], helper::N);
#elif defined(TILED)
  tiledGemm(256, 512, 8);
#else
  vanillaGemm();
#endif

  f64_t tEnd = getTime();
  f64_t duration = tEnd - tStart;

  printTime(duration);
  printFlops(duration);

  if (std::ifstream(".debug").good())
    helper::printMatrix(&matC[0][0], helper::M, helper::N);
}
