#include "default_matmul_f32.h"

/*  A: [F, S]
    B: [S, T]
    C: [F, T] */     
void default_matmul_f32(float* A, 
                float* B,
                float* C,
                int F,
                int S,
                int T){
                    // program
                    for(int f=0;f<F;++f){
                        for(int t=0;t<T;++t){
                            float sum=0.;
                            for(int s=0;s<S;s+=1){
                                sum += A[f*S+s]*B[s*T+t];
                            }
                            C[f*T + t] = sum;
                        }
                    }
                }