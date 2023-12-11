#ifdef ARM_NEON
#include <arm_neon.h>
#endif

#include "matmul_neon.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* SIMD matmul operator */


#ifdef WINDOWS
void print_mat(float* mat, int m, int n){
    for (int mid = 0; mid < m; ++mid){
        for (int nid = 0; nid <n ; ++nid){
            printf("%.3f ", mat[mid * n + nid]);
        }
        printf("\n");
    }
    printf("\n");
}
#endif

void mat_init(float* mat, int m, int n){
    for (int mid = 0; mid < m; ++mid){
        for (int nid = 0; nid <n ; ++nid){
            mat[mid * n + nid] = mid * 10. + 0.1*nid;
        }
    }
}

int mat_comp(float* mat1, float* mat2, int m, int n){
    int ans = 1;
    for (int mid = 0; mid < m; ++mid){
        for (int nid = 0; nid <n ; ++nid){
            if (fabs(mat1[mid * n + nid] - mat2[mid * n + nid]) > 1e-4){
                ans = 0;
                printf("position (%d, %d) does not match!\n", mid, nid);
                break;
            }
        }
    }
    return ans;
}

void matmul_baseline(float* mat1, float* mat2, float* mat_out,
                        int m, int n, int k){
    // row-major
    // mat1 [m,n], [mid * n + nid]
    // mat2 [n,k], [nid * k + kid]
    // mat_out [m,k], [mid * k, kid]
    for (int mid = 0; mid < m; ++mid){
        for (int kid = 0; kid < k; ++kid){
            float sum = 0.;
            for (int nid = 0; nid < n; ++nid){
                sum += mat1[mid * n + nid] * mat2[nid * k + kid];
            }
            mat_out[mid * k + kid] = sum;
        }
    }
    return;
}

#ifdef ARM_NEON
#ifdef _AARCH32
/* Substitution for vfmaq_laneq_f32 on aarch32 */
__extension__ extern __inline float32x4_t
__attribute__  ((__always_inline__, __gnu_inline__, __artificial__))
vfmaq_laneq_f32(float32x4_t Vd, float32x4_t Vn, float32x4_t Vm, int lane){
    return vaddq_f32(Vd, vmulq_n_f32(Vn, vgetq_lane_f32(Vm,lane)));
}
#endif

/* The matrix size must be 4 multiplier on both dimensions. */
void matmulq_neon(float* mat1, float* mat2, float* mat_out,
        int m, int n, int k){
    int mat1_id;
    int mat2_id;
    int mat3_id;
    // We have 31 general purpose vector registers. Don't worry!
    // row of the first matrix
    float32x4_t A0;
    float32x4_t A1;
    float32x4_t A2;
    float32x4_t A3;
    // row of the second matrix
    float32x4_t B0;
    float32x4_t B1;
    float32x4_t B2;
    float32x4_t B3;
    // row of the third matrix
    float32x4_t C0;
    float32x4_t C1;
    float32x4_t C2;
    float32x4_t C3;
    // calculation
    for (int mid = 0; mid < m; mid += 4){
        for (int kid = 0; kid < k; kid += 4){
            // accumulate the mat_out submatrix with 0, in case of uninitialized case!
            C0 = vmovq_n_f32(0);
            C1 = vmovq_n_f32(0);
            C2 = vmovq_n_f32(0);
            C3 = vmovq_n_f32(0);
            for (int nid = 0; nid < n; nid += 4){
                // the index of the upleft corner.
                mat1_id = mid * n + nid;
                mat2_id = nid * k + kid;
                // load the rows of the second matrix.
                B0 = vld1q_f32(mat2+mat2_id);
                B1 = vld1q_f32(mat2+mat2_id+k);
                B2 = vld1q_f32(mat2+mat2_id+2*k);
                B3 = vld1q_f32(mat2+mat2_id+3*k);
                // ma the first row of the first matrix and rows of the second one.
                A0 = vld1q_f32(mat1+mat1_id);
                C0 = vfmaq_laneq_f32(C0, B0, A0, 0);
                C0 = vfmaq_laneq_f32(C0, B1, A0, 1);
                C0 = vfmaq_laneq_f32(C0, B2, A0, 2);
                C0 = vfmaq_laneq_f32(C0, B3, A0, 3);
                // ma the second row of the first matrix and rows of the second one.
                A1 = vld1q_f32(mat1+mat1_id+n);
                C1 = vfmaq_laneq_f32(C1, B0, A1, 0);
                C1 = vfmaq_laneq_f32(C1, B1, A1, 1);
                C1 = vfmaq_laneq_f32(C1, B2, A1, 2);
                C1 = vfmaq_laneq_f32(C1, B3, A1, 3);
                // ma the thrid row of the first matrix and rows of the second one.
                A2 = vld1q_f32(mat1+mat1_id+2*n);
                C2 = vfmaq_laneq_f32(C2, B0, A2, 0);
                C2 = vfmaq_laneq_f32(C2, B1, A2, 1);
                C2 = vfmaq_laneq_f32(C2, B2, A2, 2);
                C2 = vfmaq_laneq_f32(C2, B3, A2, 3);
                // ma the fourth row of the first matrix and rows of the second one.
                A3 = vld1q_f32(mat1+mat1_id+3*n);
                C3 = vfmaq_laneq_f32(C3, B0, A3, 0);
                C3 = vfmaq_laneq_f32(C3, B1, A3, 1);
                C3 = vfmaq_laneq_f32(C3, B2, A3, 2);
                C3 = vfmaq_laneq_f32(C3, B3, A3, 3);
            }
            // finish calulation and store the answer.
            mat3_id = mid * k + kid;
            vst1q_f32(mat_out+mat3_id, C0);
            vst1q_f32(mat_out+mat3_id+k, C1);
            vst1q_f32(mat_out+mat3_id+2*k, C2);
            vst1q_f32(mat_out+mat3_id+3*k, C3);
        }
    }
}
#endif

int test_matmul(int m, int n, int k){
    float* mat1 = malloc(sizeof(float)*m*n);
    float* mat2 = malloc(sizeof(float)*n*k);
    float* mat_out = malloc(sizeof(float)*m*k);
    float* mat_out_neon = malloc(sizeof(float)*m*k);
    int isCorrect = 1;
    mat_init(mat1, m, n);
    mat_init(mat2, n, k);
    matmul_baseline(mat1, mat2, mat_out, m, n, k);
#ifdef WINDOWS
    print_mat(mat_out, m, k);
    isCorrect = mat_comp(mat_out, mat_out, m, k);
    printf("%d", isCorrect);
#endif
#ifdef ARM_NEON
    matmulq_neon(mat1, mat2, mat_out_neon, m, n, k);
    isCorrect = mat_comp(mat_out, mat_out_neon, m, k);
    printf("isCorrect: %d\n", isCorrect);
    if (isCorrect==1){
        printf("Test Succeeded! The SIMD Implementation is Equivalent to SISD!\n");
    }
    else{
        printf("Test Failed! The SIMD Implementation is incorrect!\n");
    }
#endif
    free(mat1);
    free(mat2);
    free(mat_out);
    free(mat_out_neon);
    return 0;
}