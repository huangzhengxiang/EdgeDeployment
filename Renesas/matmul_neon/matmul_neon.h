#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef WINDOWS
void print_mat(float* mat, int m, int n);
#endif

void mat_init(float* mat, int m, int n);
int mat_comp(float* mat1, float* mat2, int m, int n);
void matmul_baseline(float* mat1, float* mat2, float* mat_out, int m, int n, int k);

#ifdef ARM_NEON
void matmulq_neon(float* mat1, float* mat2, float* mat_out, int m, int n, int k);
#endif

int test_matmul(int m, int n, int k);