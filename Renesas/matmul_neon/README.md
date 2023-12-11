## Configuration for matmul_neon API

Configurations that shall be made:

- <b>configuration.xml</b>: Nothing changed.
- <b>C/C++ Settings</b>:  Define 2 macros, `ARM_NEON` and `_AARCH32`.
- <b>code copying</b>: put files in this directory into `src`.
- <b>setup steps</b>: None.
- Finally, build your code and debug it!

## function calls
- <b>mat_init</b>: initiate a matrix with given shape!
- <b>mat_comp</b>: compare if 2 matrices are equal. (ensure: same shape.)
- <b>matmul_baseline</b>: baseline implementation of matrix multiplication.
- <b>matmulq_neon</b>: A neon version of matrix multiplication.
- <b>test_matmul</b>: A testing API for neon implementation.