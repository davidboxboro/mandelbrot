# Mandelbrot Set Rendering

## Optimizations
- set aggressive compiler optimization flags
- parallelize for loop over pixels with OpenMP (works on LLVM but not Apple Clang)
- iterate over y then x in nested for loops for cache efficiency
- vectorize groups of 8 pixels SIMD 256
- don't use std::norm() because the square root operation is expensive (use x^2 + y^2)
- precompute all possible sf::Colors at beginning and return only pointers to them in get_color_simd()
- use float operations on real and imaginary parts rather than std::complex
- inline frequently used functions (marginal improvements)
