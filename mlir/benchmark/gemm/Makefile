CLANG = clang++
GCC = g++
CXXFLAGS = -O3 -ffast-math

OUTPUT_DIR = output

all: clang clang_ikl gcc gcc_ikl tiled tiled_copy blis openblas mkl

clang: input/gemm.cpp lib/helper.o
	$(CLANG) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.clang -lm

clang_ikl: input/gemm.cpp lib/helper.o
	$(CLANG) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.clang_ikl -lm -DIKL

gcc: input/gemm.cpp lib/helper.o
	$(GCC) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.gcc -lm

gcc_ikl: input/gemm.cpp lib/helper.o
	$(GCC) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.gcc_ikl -lm -DIKL

tiled: input/gemm.cpp lib/helper.o
	$(GCC) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.tiled -lm -DTILED

tiled_copy: input/gemm.cpp lib/helper.o
	$(GCC) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.tiled_copy -lm -DTILED -DCOPY

blis: input/gemm.cpp lib/helper.o
	$(GCC) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.blis -lblis -DBLIS

openblas: input/gemm.cpp lib/helper.o
	$(GCC) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.openblas -lopenblas -DOPENBLAS

mkl: input/gemm.cpp lib/helper.o
	$(GCC) $(CXXFLAGS) $^ -o $(OUTPUT_DIR)/gemm.mkl -lmkl_rt -DMKL

lib/helper.so: lib/helper.o
	$(GCC) -shared -o $@ $^

lib/helper.o: input/helper.cpp
	$(GCC) $(CXXFLAGS) -c $^ -o $@
