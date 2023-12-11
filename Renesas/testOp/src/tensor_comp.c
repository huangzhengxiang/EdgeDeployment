/*
 * tensor_comp.c
 *
 *  Created on: 2023年9月20日
 *      Author: hzx
 */

#include "testOp.h"

int tensor_comp_no_map(int8_t* mat1, int8_t* mat2, int len){
    int ans = 1;
    for (int32_t j = 0; j<len; ++j){
        if (mat1[j] != mat2[j]){
            ans = 0; break;
        }
    }
    return ans;
}

#ifdef NEON_AARCH32
#include <arm_neon.h>
#include <vector_functions.h>
#endif


//tinyengine_status depthwise_kernel3x3_stride1_inplace_CHW_fpreq_aarch32(q7_t *input, const uint16_t input_x, const uint16_t input_y,
//                const uint16_t input_ch, const q7_t *kernel, const int32_t *bias, const int32_t *biasR,
//                const float *scales,
//                const int32_t output_offset, const int32_t input_offset,
//                const int32_t output_activation_min,
//                const int32_t output_activation_max, q7_t *output,
//                const uint16_t output_x, const uint16_t output_y,
//                const uint16_t output_ch, q15_t *kbuf, q15_t *runtime_buf, q7_t pad_value)
//{
//
//    uint16_t c,i,j;
//    q7_t *cols_8b_start = (q7_t *)runtime_buf;
//
//    //Set padding value
//    q7_t PAD8 = pad_value;
//
//    const q7_t *src;
//    const q7_t *ksrc = kernel;
//    c=0;
//
//#ifdef NEON_AARCH32
//    q15_t* cols_16b = (q15_t* )cols_8b_start;
//
//    /* setup the padding regions for Im2col buffers */
//    int16x4_t pad16x4 = vmov_n_s16((int16_t)PAD8);
//
//    //top region: 16bit x (input_x + pad_w * 2) x pad_h: unroll by pad value
//    for(i = 0; i < input_x + 2; i++){
//        vst1_s16(cols_16b,pad16x4);
//        cols_16b+=4;
//    }
//
//    //middle regions: left and right regions
//    for(i = 0; i < input_y; i++){
//        vst1_s16(cols_16b,pad16x4);
//        cols_16b+=4;//left
//
//        cols_16b += input_x*4; //skip middle
//
//        vst1_s16(cols_16b,pad16x4);
//        cols_16b+=4;//right
//    }
//
//    //bottom region: 16bit x (input_x + pad_w * 2) x pad_h: unroll by pad value
//    for(i = 0; i < input_x + 2; i++){
//        vst1_s16(cols_16b,pad16x4);
//        cols_16b+=4;
//    }
//
//    while (c+4 <= input_ch){
//        cols_16b = (q15_t*)cols_8b_start + (1 * (input_x) + 2)*4; //skip 1 rows
//        src = input;
//
//        /* load the kernel */
//        for (int cid=0; cid<4; ++cid){
//            for (int hid=0; hid<3; ++hid){
//                for (int wid=0; wid<3; ++wid){
//                    kbuf[hid*3*4+wid*4+cid]=(q15_t)ksrc[cid*3*3+hid*3+wid];
//                }
//            }
//        }
//
//        for(i = 0; i < input_y; i++){
//            cols_16b += 4;//skip front
//            j=0;
//            while(j<input_x){
//                vst1_s16(cols_16b,vget_low_s16(vmovl_s8(vld1_s8(src))));// + input_offset;
//                src += input_ch;
//                cols_16b += 4;
//                j++;
//            }
//            cols_16b += 4;//skip end
//        }
//
//        q7_t *inplace_out = input;
//        depthwise_kernel3x3_stride1_inplace_kernel_CHW_fpreq_aarch32(output_y, output_x, bias, biasR, (q7_t*)kbuf, scales, inplace_out, output_offset,output_activation_min, output_activation_max,cols_8b_start, input_x, input_ch);
//
//        bias+=4;
//        biasR+=4;
//        scales+=4;
//
//        ksrc += 4*3*3;
//        input += 4;
//        c += 4;
//    }
//#endif
//
//    if (c < input_ch){
//
//        q7_t* cols_8b = (q7_t* )cols_8b_start;
//        /* setup the padding regions for Im2col buffers */
//        //top region: 8bit x (input_x + pad_w * 2) x pad_h: unroll by pad value
//        for(i = 0; i < input_x + 2; i++){
//            *cols_8b++ = PAD8;
//        }
//
//        //middle regions: left and right regions
//        for(i = 0; i < input_y; i++){
//            *cols_8b++ = PAD8;//left
//            cols_8b += input_x; //skip middle
//            *cols_8b++ = PAD8;//right
//        }
//
//        //bottom region: 8bit x (input_x + pad_w * 2) x pad_h: unroll by pad value
//        for(i = 0; i < input_x + 2; i++){
//            *cols_8b++ = PAD8;
//        }
//
//        while(c < input_ch){
//            cols_8b = (q7_t*)(cols_8b_start + 1 * (input_x) + 2); //skip 1 rows
//            src = input;
//            for(i = 0; i < input_y; i++){
//                cols_8b += 1;//skip front
//                j=0;
//                while(j<input_x){
//                    *cols_8b++ = *src;// + input_offset;
//                    src += input_ch;
//                    j++;
//                }
//                cols_8b += 1;//skip end
//            }
//            q7_t *inplace_out = input;
//            depthwise_kernel3x3_stride1_inplace_kernel_CHW_fpreq_quick(output_y, output_x, bias++, biasR++, ksrc, scales++, inplace_out, output_offset,output_activation_min, output_activation_max,cols_8b_start, input_x, input_ch);
//            ksrc += 3*3;
//            input++;
//            c++;
//        }
//    }
//    return STATE_SUCCESS;
//}
//void depthwise_kernel3x3_stride1_inplace_kernel_CHW_fpreq_quick(
//        const uint16_t output_y, const uint16_t output_x,
//        const int32_t *bias, const int32_t *biasR, const q7_t *ksrc, const float *scales,
//        q7_t *output, const int32_t output_offset,
//        const int32_t activation_min, const int32_t activation_max,
//        q7_t *cols_8b_iterptr, const uint16_t column_x, int channel_offset)
//{
//    #define STRIDE 1
//    int i, j;
//    int16_t input_buf;
//    /* MACs for each output */
//    for (i = 0; i < output_y; i++) {
//        j=0;
//        while(j+4<=output_x) {
//            q7_t *cols_8b = cols_8b_iterptr;
//
//            q31_t sum0 = bias[0] + biasR[0];
//            q31_t sum1 = bias[0] + biasR[0];
//            q31_t sum2 = bias[0] + biasR[0];
//            q31_t sum3 = bias[0] + biasR[0];
//
//            /* computation */
//            sum0 += ((int16_t)cols_8b[0])*ksrc[0];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[0];
//            sum0 += input_buf*ksrc[1];
//            input_buf = (int16_t)cols_8b[2];
//            sum2 += input_buf*ksrc[0];
//            sum1 += input_buf*ksrc[1];
//            sum0 += input_buf*ksrc[2];
//            input_buf = (int16_t)cols_8b[3];
//            sum3 += input_buf*ksrc[0];
//            sum2 += input_buf*ksrc[1];
//            sum1 += input_buf*ksrc[2];
//            input_buf = (int16_t)cols_8b[4];
//            sum3 += input_buf*ksrc[1];
//            sum2 += input_buf*ksrc[2];
//            sum3 += ((int16_t)cols_8b[5])*ksrc[2];
//            cols_8b += column_x + 2;
//
//            sum0 += cols_8b[0]*ksrc[3];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[3];
//            sum0 += input_buf*ksrc[4];
//            input_buf = (int16_t)cols_8b[2];
//            sum2 += input_buf*ksrc[3];
//            sum1 += input_buf*ksrc[4];
//            sum0 += input_buf*ksrc[5];
//            input_buf = (int16_t)cols_8b[3];
//            sum3 += input_buf*ksrc[3];
//            sum2 += input_buf*ksrc[4];
//            sum1 += input_buf*ksrc[5];
//            input_buf = (int16_t)cols_8b[4];
//            sum3 += input_buf*ksrc[4];
//            sum2 += input_buf*ksrc[5];
//            sum3 += ((int16_t)cols_8b[5])*ksrc[5];
//            cols_8b += column_x + 2;
//
//            sum0 += cols_8b[0]*ksrc[6];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[6];
//            sum0 += input_buf*ksrc[7];
//            input_buf = (int16_t)cols_8b[2];
//            sum2 += input_buf*ksrc[6];
//            sum1 += input_buf*ksrc[7];
//            sum0 += input_buf*ksrc[8];
//            input_buf = (int16_t)cols_8b[3];
//            sum3 += input_buf*ksrc[6];
//            sum2 += input_buf*ksrc[7];
//            sum1 += input_buf*ksrc[8];
//            input_buf = (int16_t)cols_8b[4];
//            sum3 += input_buf*ksrc[7];
//            sum2 += input_buf*ksrc[8];
//            sum3 += ((int16_t)cols_8b[5])*ksrc[8];
//
//            /* requantize */
//            sum0 = (float) sum0 * *scales;
//            sum0 += output_offset;
//            sum0 = TN_MAX(sum0, activation_min);
//            sum0 = TN_MIN(sum0, activation_max);
//            output[(i * output_x + j) * channel_offset] = sum0;
//
//            sum1 = (float) sum1 * *scales;
//            sum1 += output_offset;
//            sum1 = TN_MAX(sum1, activation_min);
//            sum1 = TN_MIN(sum1, activation_max);
//            output[(i * output_x + (j + 1)) * channel_offset] = sum1;
//
//            sum2 = (float) sum2 * *scales;
//            sum2 += output_offset;
//            sum2 = TN_MAX(sum2, activation_min);
//            sum2 = TN_MIN(sum2, activation_max);
//            output[(i * output_x + (j + 2)) * channel_offset] = sum2;
//
//            sum3 = (float) sum3 * *scales;
//            sum3 += output_offset;
//            sum3 = TN_MAX(sum3, activation_min);
//            sum3 = TN_MIN(sum3, activation_max);
//            output[(i * output_x + (j + 3)) * channel_offset] = sum3;
//
//            cols_8b_iterptr += STRIDE * 4;
//
//            j+=4;
//        }
//        if(j+3<=output_x) {
//            q7_t *cols_8b = cols_8b_iterptr;
//
//            q31_t sum0 = bias[0] + biasR[0];
//            q31_t sum1 = bias[0] + biasR[0];
//            q31_t sum2 = bias[0] + biasR[0];
//
//            /* computation */
//            sum0 += ((int16_t)cols_8b[0])*ksrc[0];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[0];
//            sum0 += input_buf*ksrc[1];
//            input_buf = (int16_t)cols_8b[2];
//            sum2 += input_buf*ksrc[0];
//            sum1 += input_buf*ksrc[1];
//            sum0 += input_buf*ksrc[2];
//            input_buf = (int16_t)cols_8b[3];
//            sum2 += input_buf*ksrc[1];
//            sum1 += input_buf*ksrc[2];
//            sum2 += ((int16_t)cols_8b[4])*ksrc[2];
//            cols_8b += column_x + 2;
//
//            sum0 += cols_8b[0]*ksrc[3];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[3];
//            sum0 += input_buf*ksrc[4];
//            input_buf = (int16_t)cols_8b[2];
//            sum2 += input_buf*ksrc[3];
//            sum1 += input_buf*ksrc[4];
//            sum0 += input_buf*ksrc[5];
//            input_buf = (int16_t)cols_8b[3];
//            sum2 += input_buf*ksrc[4];
//            sum1 += input_buf*ksrc[5];
//            sum2 += ((int16_t)cols_8b[4])*ksrc[5];
//            cols_8b += column_x + 2;
//
//            sum0 += cols_8b[0]*ksrc[6];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[6];
//            sum0 += input_buf*ksrc[7];
//            input_buf = (int16_t)cols_8b[2];
//            sum2 += input_buf*ksrc[6];
//            sum1 += input_buf*ksrc[7];
//            sum0 += input_buf*ksrc[8];
//            input_buf = (int16_t)cols_8b[3];
//            sum2 += input_buf*ksrc[7];
//            sum1 += input_buf*ksrc[8];
//            sum2 += ((int16_t)cols_8b[4])*ksrc[8];
//
//            /* requantize */
//            sum0 = (float) sum0 * *scales;
//            sum0 += output_offset;
//            sum0 = TN_MAX(sum0, activation_min);
//            sum0 = TN_MIN(sum0, activation_max);
//            output[(i * output_x + j) * channel_offset] = sum0;
//
//            sum1 = (float) sum1 * *scales;
//            sum1 += output_offset;
//            sum1 = TN_MAX(sum1, activation_min);
//            sum1 = TN_MIN(sum1, activation_max);
//            output[(i * output_x + (j + 1)) * channel_offset] = sum1;
//
//            sum2 = (float) sum2 * *scales;
//            sum2 += output_offset;
//            sum2 = TN_MAX(sum2, activation_min);
//            sum2 = TN_MIN(sum2, activation_max);
//            output[(i * output_x + (j + 2)) * channel_offset] = sum2;
//
//            cols_8b_iterptr += STRIDE * 3;
//
//            j+=3;
//        }
//        if (j+2<=output_x) {
//            q7_t *cols_8b = cols_8b_iterptr;
//
//            q31_t sum0 = bias[0] + biasR[0];
//            q31_t sum1 = bias[0] + biasR[0];
//
//            /* computation */
//            sum0 += ((int16_t)cols_8b[0])*ksrc[0];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[0];
//            sum0 += input_buf*ksrc[1];
//            input_buf = (int16_t)cols_8b[2];
//            sum1 += input_buf*ksrc[1];
//            sum0 += input_buf*ksrc[2];
//            sum1 += ((int16_t)cols_8b[3])*ksrc[2];
//            cols_8b += column_x + 2;
//
//            sum0 += cols_8b[0]*ksrc[3];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[3];
//            sum0 += input_buf*ksrc[4];
//            input_buf = (int16_t)cols_8b[2];
//            sum1 += input_buf*ksrc[4];
//            sum0 += input_buf*ksrc[5];
//            sum1 += ((int16_t)cols_8b[3])*ksrc[5];
//            cols_8b += column_x + 2;
//
//            sum0 += cols_8b[0]*ksrc[6];
//            input_buf = (int16_t)cols_8b[1];
//            sum1 += input_buf*ksrc[6];
//            sum0 += input_buf*ksrc[7];
//            input_buf = (int16_t)cols_8b[2];
//            sum1 += input_buf*ksrc[7];
//            sum0 += input_buf*ksrc[8];
//            sum1 += ((int16_t)cols_8b[3])*ksrc[8];
//
//            /* requantize */
//            sum0 = (float) sum0 * *scales;
//            sum0 += output_offset;
//            sum0 = TN_MAX(sum0, activation_min);
//            sum0 = TN_MIN(sum0, activation_max);
//            output[(i * output_x + j) * channel_offset] = sum0;
//
//            sum1 = (float) sum1 * *scales;
//            sum1 += output_offset;
//            sum1 = TN_MAX(sum1, activation_min);
//            sum1 = TN_MIN(sum1, activation_max);
//            output[(i * output_x + (j + 1)) * channel_offset] = sum1;
//
//            cols_8b_iterptr += STRIDE * 2;
//
//            j+=2;
//        }
//        if (j<output_x) {
//            q7_t * cols_8b = cols_8b_iterptr;
//            q31_t sum = bias[0] + biasR[0];
//            sum += cols_8b[0]*ksrc[0];
//            sum += cols_8b[1]*ksrc[1];
//            sum += cols_8b[2]*ksrc[2];
//            cols_8b += column_x + 2;
//            sum += cols_8b[0]*ksrc[3];
//            sum += cols_8b[1]*ksrc[4];
//            sum += cols_8b[2]*ksrc[5];
//            cols_8b += column_x + 2;
//            sum += cols_8b[0]*ksrc[6];
//            sum += cols_8b[1]*ksrc[7];
//            sum += cols_8b[2]*ksrc[8];
//
//            sum = (float) sum * *scales;
//            sum += output_offset;
//            sum = TN_MAX(sum, activation_min);
//            sum = TN_MIN(sum, activation_max);
//            output[(i * output_x + output_x - 1) * channel_offset] = sum;
//
//            cols_8b_iterptr += STRIDE;
//        }
//        cols_8b_iterptr += 1 * 2;
//    }
//}
//
//void depthwise_kernel3x3_stride1_inplace_kernel_CHW_fpreq_aarch32(
//        const uint16_t output_y, const uint16_t output_x,
//        const int32_t *bias, const int32_t *biasR, const q7_t *ksrc, const float *scales,
//        q7_t *output, const int32_t output_offset,
//        const int32_t activation_min, const int32_t activation_max,
//        q7_t *cols_8b_iterptr, const uint16_t column_x, int channel_offset)
//{
//    #define STRIDE 1
//    int i, j;
//    int16x4_t input_buf;
//    int16_t buffer[4];
//    int16x4_t kernel0;
//    int16x4_t kernel1;
//    int16x4_t kernel2;
//
//    int32x4_t biasx4 = vaddq_s32(vld1q_s32(bias),vld1q_s32(biasR));
//    float32x4_t scalesx4 = vld1q_f32(scales);
//    int32x4_t output_offsetx4 = vmovq_n_s32(output_offset);
//    int32x4_t activation_maxx4 = vmovq_n_s32(activation_max);
//    int32x4_t activation_minx4 = vmovq_n_s32(activation_min);
//
//    int32x4_t sum0;
//    int32x4_t sum1;
//    int32x4_t sum2;
//    int32x4_t sum3;
//
//    q15_t* kbuf = (q15_t*)ksrc;
//    q15_t* cols_16b_iterptr = (q15_t*)cols_8b_iterptr;
//
//    /* MACs for each output */
//    for (i = 0; i < output_y; i++) {
//        j=0;
//        while(j+4<=output_x) {
//            q15_t *cols_16b = cols_16b_iterptr;
//
//            sum0 = biasx4;
//            sum1 = biasx4;
//            sum2 = biasx4;
//            sum3 = biasx4;
//
//            /* computation */
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[0]));
//            kernel1 = vld1_s16(&(kbuf[4]));
//            kernel2 = vld1_s16(&(kbuf[8]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel0);
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel0);
//            sum2 = vmlal_s16(sum2, input_buf, kernel1);
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[16]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel1);
//            sum2 = vmlal_s16(sum2, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[20]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[12]));
//            kernel1 = vld1_s16(&(kbuf[16]));
//            kernel2 = vld1_s16(&(kbuf[20]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel0);
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel0);
//            sum2 = vmlal_s16(sum2, input_buf, kernel1);
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[16]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel1);
//            sum2 = vmlal_s16(sum2, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[20]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[24]));
//            kernel1 = vld1_s16(&(kbuf[28]));
//            kernel2 = vld1_s16(&(kbuf[32]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel0);
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel0);
//            sum2 = vmlal_s16(sum2, input_buf, kernel1);
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[16]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel1);
//            sum2 = vmlal_s16(sum2, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[20]));
//            sum3 = vmlal_s16(sum3, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            /* requantize */
//            sum0 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum0),scalesx4)),output_offsetx4);
//            sum1 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum1),scalesx4)),output_offsetx4);
//            sum2 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum2),scalesx4)),output_offsetx4);
//            sum3 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum3),scalesx4)),output_offsetx4);
//            sum0 = vmaxq_s32(sum0, activation_minx4);
//            sum1 = vmaxq_s32(sum1, activation_minx4);
//            sum2 = vmaxq_s32(sum2, activation_minx4);
//            sum3 = vmaxq_s32(sum3, activation_minx4);
//            sum0 = vminq_s32(sum0, activation_maxx4);
//            sum1 = vminq_s32(sum1, activation_maxx4);
//            sum2 = vminq_s32(sum2, activation_maxx4);
//            sum3 = vminq_s32(sum3, activation_maxx4);
//            vstn_s16x4(&(output[(i * output_x + j) * channel_offset]), buffer, vmovn_s32(sum0));
//            vstn_s16x4(&(output[(i * output_x + j+1) * channel_offset]), buffer, vmovn_s32(sum1));
//            vstn_s16x4(&(output[(i * output_x + j+2) * channel_offset]), buffer, vmovn_s32(sum2));
//            vstn_s16x4(&(output[(i * output_x + j+3) * channel_offset]), buffer, vmovn_s32(sum3));
//
//            cols_16b_iterptr += (STRIDE * 4)*4;
//
//            j+=4;
//        }
//        if(j+3<=output_x) {
//            q15_t *cols_16b = cols_16b_iterptr;
//
//            sum0 = biasx4;
//            sum1 = biasx4;
//            sum2 = biasx4;
//
//            /* computation */
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[0]));
//            kernel1 = vld1_s16(&(kbuf[4]));
//            kernel2 = vld1_s16(&(kbuf[8]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel0);
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel1);
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[16]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[12]));
//            kernel1 = vld1_s16(&(kbuf[16]));
//            kernel2 = vld1_s16(&(kbuf[20]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel0);
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel1);
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[16]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[24]));
//            kernel1 = vld1_s16(&(kbuf[28]));
//            kernel2 = vld1_s16(&(kbuf[32]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel0);
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel1);
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[16]));
//            sum2 = vmlal_s16(sum2, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            /* requantize */
//            sum0 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum0),scalesx4)),output_offsetx4);
//            sum1 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum1),scalesx4)),output_offsetx4);
//            sum2 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum2),scalesx4)),output_offsetx4);
//            sum0 = vmaxq_s32(sum0, activation_minx4);
//            sum1 = vmaxq_s32(sum1, activation_minx4);
//            sum2 = vmaxq_s32(sum2, activation_minx4);
//            sum0 = vminq_s32(sum0, activation_maxx4);
//            sum1 = vminq_s32(sum1, activation_maxx4);
//            sum2 = vminq_s32(sum2, activation_maxx4);
//            vstn_s16x4(&(output[(i * output_x + j) * channel_offset]), buffer, vmovn_s32(sum0));
//            vstn_s16x4(&(output[(i * output_x + j+1) * channel_offset]), buffer, vmovn_s32(sum1));
//            vstn_s16x4(&(output[(i * output_x + j+2) * channel_offset]), buffer, vmovn_s32(sum2));
//
//            cols_16b_iterptr += (STRIDE * 3)*4;
//
//            j+=3;
//        }
//        if (j+2<=output_x) {
//            q15_t *cols_16b = cols_16b_iterptr;
//
//            sum0 = biasx4;
//            sum1 = biasx4;
//
//            /* computation */
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[0]));
//            kernel1 = vld1_s16(&(kbuf[4]));
//            kernel2 = vld1_s16(&(kbuf[8]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[12]));
//            kernel1 = vld1_s16(&(kbuf[16]));
//            kernel2 = vld1_s16(&(kbuf[20]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[24]));
//            kernel1 = vld1_s16(&(kbuf[28]));
//            kernel2 = vld1_s16(&(kbuf[32]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel0);
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel1);
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            input_buf = vld1_s16(&(cols_16b[12]));
//            sum1 = vmlal_s16(sum1, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            /* requantize */
//            sum0 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum0),scalesx4)),output_offsetx4);
//            sum1 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum1),scalesx4)),output_offsetx4);
//            sum0 = vmaxq_s32(sum0, activation_minx4);
//            sum1 = vmaxq_s32(sum1, activation_minx4);
//            sum0 = vminq_s32(sum0, activation_maxx4);
//            sum1 = vminq_s32(sum1, activation_maxx4);
//            vstn_s16x4(&(output[(i * output_x + j) * channel_offset]), buffer, vmovn_s32(sum0));
//            vstn_s16x4(&(output[(i * output_x + j+1) * channel_offset]), buffer, vmovn_s32(sum1));
//
//            cols_16b_iterptr += (STRIDE * 2)*4;
//
//            j+=2;
//        }
//        if (j<output_x) {
//            q15_t *cols_16b = cols_16b_iterptr;
//
//            sum0 = biasx4;
//
//            /* computation */
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[0]));
//            kernel1 = vld1_s16(&(kbuf[4]));
//            kernel2 = vld1_s16(&(kbuf[8]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[12]));
//            kernel1 = vld1_s16(&(kbuf[16]));
//            kernel2 = vld1_s16(&(kbuf[20]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            input_buf = vld1_s16(&(cols_16b[0]));
//            kernel0 = vld1_s16(&(kbuf[24]));
//            kernel1 = vld1_s16(&(kbuf[28]));
//            kernel2 = vld1_s16(&(kbuf[32]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel0);
//            input_buf = vld1_s16(&(cols_16b[4]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel1);
//            input_buf = vld1_s16(&(cols_16b[8]));
//            sum0 = vmlal_s16(sum0, input_buf, kernel2);
//            cols_16b += (column_x + 2)*4;
//
//            /* requantize */
//            sum0 = vaddq_s32(vcvtq_s32_f32(vmulq_f32(vcvtq_f32_s32(sum0),scalesx4)),output_offsetx4);
//            sum0 = vmaxq_s32(sum0, activation_minx4);
//            sum0 = vminq_s32(sum0, activation_maxx4);
//            vstn_s16x4(&(output[(i * output_x + j) * channel_offset]), buffer, vmovn_s32(sum0));
//
//            cols_16b_iterptr += STRIDE *4;
//        }
//        cols_16b_iterptr += (1 * 2)*4;
//    }
//}

/* This file is automatically generated */
/* ----------------------------------------------------------------------
 * Project:      TinyEngine
 * Description:  for sparse in-place 5x5 depth-wise convolution (HWC->CHW->HWC)
 * Target ISA:  ARMv7E-M
 * Author: wmchen@mit.edu
 * -------------------------------------------------------------------- */
#include "tinyengine_function.h"
void depthwise_kernel5x5_stride1_inplace_kernel_CHW_fpreq_ori(
        const uint16_t output_y, const uint16_t output_x,
        const int32_t *bias, const int32_t *biasR, const q7_t *ksrc, const float *scales,
        q7_t *output, const int32_t output_offset,
        const int32_t activation_min, const int32_t activation_max,
        q7_t *cols_8b_iterptr, const uint16_t column_x, int channel_offset);
tinyengine_status depthwise_kernel5x5_stride1_inplace_CHW_fpreq_ori(q7_t *input, const uint16_t input_x, const uint16_t input_y,
                const uint16_t input_ch, const q7_t *kernel, const int32_t *bias, const int32_t *biasR,
                const float *scales,
                const int32_t output_offset, const int32_t input_offset,
                const int32_t output_activation_min,
                const int32_t output_activation_max, q7_t *output,
                const uint16_t output_x, const uint16_t output_y,
                const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value)
{

    uint16_t c,i,j;
    q7_t *cols_8b_start = (q7_t *)runtime_buf;
    q7_t* cols_8b = (q7_t* )cols_8b_start;

    //Set padding value
    q7_t PAD8 = pad_value;
    /* setup the padding regions for Im2col buffers */
    //top region: 8bit x (input_x + pad_w * 2) x pad_h: unroll by pad value
    for(i = 0; i < input_x + 4; i++){
        *cols_8b++ = PAD8;
        *cols_8b++ = PAD8;
    }

    //middle regions: left and right regions
    for(i = 0; i < input_y; i++){
        *cols_8b++ = PAD8;//left
        *cols_8b++ = PAD8;//left
        cols_8b += input_x; //skip middle
        *cols_8b++ = PAD8;//right
        *cols_8b++ = PAD8;//right
    }

    //bottom region: 8bit x (input_x + pad_w * 2) x pad_h: unroll by pad value
    for(i = 0; i < input_x + 4; i++){
        *cols_8b++ = PAD8;
        *cols_8b++ = PAD8;
    }

    const q7_t *src;
    const q7_t *ksrc = kernel;

    for (c = 0; c < input_ch; c++){
        cols_8b = (q7_t*)(cols_8b_start + 2 * (input_x) + 8); //skip 2 rows
        src = input;
        for(i = 0; i < input_y; i++){
            cols_8b += 2;//skip front
            for(j = 0; j < input_x; j++){
                *cols_8b++ = *src;// + input_offset;
                src += input_ch;
            }
            cols_8b += 2;//skip end
        }
        q7_t *inplace_out = input;
        depthwise_kernel5x5_stride1_inplace_kernel_CHW_fpreq_ori(output_y, output_x, bias++, biasR++, ksrc, scales++, inplace_out, output_offset,output_activation_min, output_activation_max,cols_8b_start, input_x, input_ch);
        ksrc += 25;
        input++;
    }
    return STATE_SUCCESS;
}
void depthwise_kernel5x5_stride1_inplace_kernel_CHW_fpreq_ori(
        const uint16_t output_y, const uint16_t output_x,
        const int32_t *bias, const int32_t *biasR, const q7_t *ksrc, const float *scales,
        q7_t *output, const int32_t output_offset,
        const int32_t activation_min, const int32_t activation_max,
        q7_t *cols_8b_iterptr, const uint16_t column_x, int channel_offset)
{
    #define STRIDE 1
    int i, j;
    /* MACs for each output */
    for (i = 0; i < output_y; i++) {
        for (j = 0; j < output_x / 2; j++) {
            q7_t *cols_8b = cols_8b_iterptr;

            q31_t sum0 = bias[0] + biasR[0];
            q31_t sum1 = bias[0] + biasR[0];

            /* computation */
            sum0 += cols_8b[0]*ksrc[0];
            sum1 += cols_8b[1]*ksrc[0];
            sum0 += cols_8b[1]*ksrc[1];
            sum1 += cols_8b[2]*ksrc[1];
            sum0 += cols_8b[2]*ksrc[2];
            sum1 += cols_8b[3]*ksrc[2];
            sum0 += cols_8b[3]*ksrc[3];
            sum1 += cols_8b[4]*ksrc[3];
            sum0 += cols_8b[4]*ksrc[4];
            sum1 += cols_8b[5]*ksrc[4];
            cols_8b += column_x + 4;
            sum0 += cols_8b[0]*ksrc[5];
            sum1 += cols_8b[1]*ksrc[5];
            sum0 += cols_8b[1]*ksrc[6];
            sum1 += cols_8b[2]*ksrc[6];
            sum0 += cols_8b[2]*ksrc[7];
            sum1 += cols_8b[3]*ksrc[7];
            sum0 += cols_8b[3]*ksrc[8];
            sum1 += cols_8b[4]*ksrc[8];
            sum0 += cols_8b[4]*ksrc[9];
            sum1 += cols_8b[5]*ksrc[9];
            cols_8b += column_x + 4;
            sum0 += cols_8b[0]*ksrc[10];
            sum1 += cols_8b[1]*ksrc[10];
            sum0 += cols_8b[1]*ksrc[11];
            sum1 += cols_8b[2]*ksrc[11];
            sum0 += cols_8b[2]*ksrc[12];
            sum1 += cols_8b[3]*ksrc[12];
            sum0 += cols_8b[3]*ksrc[13];
            sum1 += cols_8b[4]*ksrc[13];
            sum0 += cols_8b[4]*ksrc[14];
            sum1 += cols_8b[5]*ksrc[14];
            cols_8b += column_x + 4;
            sum0 += cols_8b[0]*ksrc[15];
            sum1 += cols_8b[1]*ksrc[15];
            sum0 += cols_8b[1]*ksrc[16];
            sum1 += cols_8b[2]*ksrc[16];
            sum0 += cols_8b[2]*ksrc[17];
            sum1 += cols_8b[3]*ksrc[17];
            sum0 += cols_8b[3]*ksrc[18];
            sum1 += cols_8b[4]*ksrc[18];
            sum0 += cols_8b[4]*ksrc[19];
            sum1 += cols_8b[5]*ksrc[19];
            cols_8b += column_x + 4;
            sum0 += cols_8b[0]*ksrc[20];
            sum1 += cols_8b[1]*ksrc[20];
            sum0 += cols_8b[1]*ksrc[21];
            sum1 += cols_8b[2]*ksrc[21];
            sum0 += cols_8b[2]*ksrc[22];
            sum1 += cols_8b[3]*ksrc[22];
            sum0 += cols_8b[3]*ksrc[23];
            sum1 += cols_8b[4]*ksrc[23];
            sum0 += cols_8b[4]*ksrc[24];
            sum1 += cols_8b[5]*ksrc[24];

            /* requantize */
            sum0 = (float) sum0 * *scales;
            sum0 += output_offset;
            sum0 = TN_MAX(sum0, activation_min);
            sum0 = TN_MIN(sum0, activation_max);
            output[(i * output_x + j * 2) * channel_offset] = sum0;

            sum1 = (float) sum1 * *scales;
            sum1 += output_offset;
            sum1 = TN_MAX(sum1, activation_min);
            sum1 = TN_MIN(sum1, activation_max);
            output[(i * output_x + (j * 2 + 1)) * channel_offset] = sum1;

            cols_8b_iterptr += STRIDE * 2;
        }
        if (output_x & 1) {
            q7_t * cols_8b = cols_8b_iterptr;
            q31_t sum = bias[0] + biasR[0];
            sum += cols_8b[0]*ksrc[0];
            sum += cols_8b[1]*ksrc[1];
            sum += cols_8b[2]*ksrc[2];
            sum += cols_8b[3]*ksrc[3];
            sum += cols_8b[4]*ksrc[4];
            cols_8b += column_x + 4;
            sum += cols_8b[0]*ksrc[5];
            sum += cols_8b[1]*ksrc[6];
            sum += cols_8b[2]*ksrc[7];
            sum += cols_8b[3]*ksrc[8];
            sum += cols_8b[4]*ksrc[9];
            cols_8b += column_x + 4;
            sum += cols_8b[0]*ksrc[10];
            sum += cols_8b[1]*ksrc[11];
            sum += cols_8b[2]*ksrc[12];
            sum += cols_8b[3]*ksrc[13];
            sum += cols_8b[4]*ksrc[14];
            cols_8b += column_x + 4;
            sum += cols_8b[0]*ksrc[15];
            sum += cols_8b[1]*ksrc[16];
            sum += cols_8b[2]*ksrc[17];
            sum += cols_8b[3]*ksrc[18];
            sum += cols_8b[4]*ksrc[19];
            cols_8b += column_x + 4;
            sum += cols_8b[0]*ksrc[20];
            sum += cols_8b[1]*ksrc[21];
            sum += cols_8b[2]*ksrc[22];
            sum += cols_8b[3]*ksrc[23];
            sum += cols_8b[4]*ksrc[24];

            sum = (float) sum * *scales;
            sum += output_offset;
            sum = TN_MAX(sum, activation_min);
            sum = TN_MIN(sum, activation_max);
            output[(i * output_x + output_x - 1) * channel_offset] = sum;

            cols_8b_iterptr += STRIDE;
        }
        cols_8b_iterptr += 2 * 2;
    }
}
