func.func @matmul(%A: memref<2048x2048xf64>, %B: memref<2048x2048xf64>,
             %C: memref<2048x2048xf64>, %alpha: f64, %beta: f64) {
    affine.for %i = 0 to 2048 {
        affine.for %j = 0 to 2048 {
            affine.for %k = 0 to 2048 {
                %a = affine.load %A[%i, %k] : memref<2048x2048xf64>
                %b = affine.load %B[%k, %j] : memref<2048x2048xf64>
                %c_in = affine.load %C[%i, %j] : memref<2048x2048xf64>
                %mul = arith.mulf %a, %b : f64
                %mul_a = arith.mulf %mul, %alpha : f64
                %mul_c = arith.mulf %c_in, %beta : f64
                %c_out = arith.addf %mul_a, %mul_c : f64
                affine.store %c_out, %C[%i, %j] : memref<2048x2048xf64>
            }
        }
    }
    return
}

func.func @main() {
    %A = memref.alloc() : memref<2048x2048xf64>
    %B = memref.alloc() : memref<2048x2048xf64>
    %C = memref.alloc() : memref<2048x2048xf64>

    %cf1 = arith.constant 1.00000e+00 : f64

    linalg.fill ins(%cf1 : f64) outs(%A : memref<2048x2048xf64>)
    linalg.fill ins(%cf1 : f64) outs(%B : memref<2048x2048xf64>)
    linalg.fill ins(%cf1 : f64) outs(%C : memref<2048x2048xf64>)

    %t_start = func.call @getTime() : () -> (f64)
    func.call @matmul(%A, %B, %C, %cf1, %cf1) : (memref<2048x2048xf64>,
                                                 memref<2048x2048xf64>,
                                                 memref<2048x2048xf64>,
                                                 f64, f64) -> ()
    %t_end = func.call @getTime() : () -> (f64)
    %time = arith.subf %t_end, %t_start : f64

    func.call @printTime(%time) : (f64) -> ()
    func.call @printFlops(%time) : (f64) -> ()

    return
}

func.func private @printTime(f64) -> ()
func.func private @printFlops(f64) -> ()
func.func private @getTime() -> (f64)
