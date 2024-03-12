/*
 * testOp.h
 *
 *  Created on: 2023年9月20日
 *      Author: hzx
 */

#ifndef TESTOP_H_
#define TESTOP_H_

#include "tinyengine_function.h"
int tensor_comp_no_map(int8_t* mat1, int8_t* mat2, int len);
//
//typedef enum{
//    sbuf_s16=0, // sbuf type is s16
//    sbuf_s8=1   //sbuf type is s8
//}sbuf_type;
//
//q7_t* mat_mult_kernel3_input3_s8_s16_fpreq_aarch32(const q7_t *input_a,
//        const q15_t *input_b, const uint16_t output_ch, const float *scales,
//        const int32_t out_offset, const int16_t activation_min,
//        const int16_t activation_max, const uint16_t num_col_a,
//        const int32_t *const output_bias, q7_t *out_0, q15_t *kbuf);
//
//tinyengine_status convolve_s8_kernel3_inputch3_stride2_pad1_fpreq_aarch32(
//        const q7_t *input, const uint16_t input_x, const uint16_t input_y,
//        const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
//        const float *scales, const int32_t output_offset,
//        const int32_t input_offset, const int32_t output_activation_min,
//        const int32_t output_activation_max, q7_t *output,
//        const uint16_t output_x, const uint16_t output_y,
//        const uint16_t output_ch, q15_t *runtime_buf, q15_t *kbuf,
//        q7_t pad_value);
//
//q7_t* mat_mult_kernel3_input3_s8_s16_fpreq_aarch32_transposed(const q7_t *input_a,
//        const q15_t *input_b, const uint16_t output_ch, const float *scales,
//        const int32_t out_offset, const int32_t input_offset,
//        const int16_t activation_min, const int16_t activation_max,
//        const uint16_t num_col_a, const int32_t *const output_bias, q7_t *out_0, q15_t *kbuf,
//        sbuf_type input_b_type_flag);
//
//tinyengine_status convolve_s8_kernel3_inputch3_stride2_pad1_fpreq_aarch32_transposed(
//        const q7_t *input, const uint16_t input_x, const uint16_t input_y,
//        const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
//        const float *scales, const int32_t output_offset,
//        const int32_t input_offset, const int32_t output_activation_min,
//        const int32_t output_activation_max, q7_t *output,
//        const uint16_t output_x, const uint16_t output_y,
//        const uint16_t output_ch, q15_t *runtime_buf, q15_t *kbuf,
//        q7_t pad_value);
//
//tinyengine_status convolve_fpreq_generic(const q7_t *input,
//        const uint16_t input_x, const uint16_t input_y, const uint16_t input_ch,
//        const q7_t *kernel, const int32_t *bias, const float *scales,
//        const int32_t out_offset, const int32_t input_offset,
//        const int32_t out_activation_min, const int32_t out_activation_max,
//        q7_t *output, const uint16_t output_x, const uint16_t output_y,
//        const uint16_t output_ch, q15_t *kbuf, q15_t *runtime_buf,
//        const int kernel_x, const int kernel_y, const int stride, const int pad, q7_t pad_value);

void depthwise_kernel3x3_stride1_inplace_kernel_CHW_fpreq_quick(
        const uint16_t output_y, const uint16_t output_x,
        const int32_t *bias, const int32_t *biasR, const q7_t *ksrc, const float *scales,
        q7_t *output, const int32_t output_offset,
        const int32_t activation_min, const int32_t activation_max,
        q7_t *cols_8b_iterptr, const uint16_t column_x, int channel_offset);

void depthwise_kernel3x3_stride1_inplace_kernel_CHW_fpreq_aarch32(
        const uint16_t output_y, const uint16_t output_x,
        const int32_t *bias, const int32_t *biasR, const q7_t *ksrc, const float *scales,
        q7_t *output, const int32_t output_offset,
        const int32_t activation_min, const int32_t activation_max,
        q7_t *cols_8b_iterptr, const uint16_t column_x, int channel_offset);

tinyengine_status depthwise_kernel3x3_stride1_inplace_CHW_fpreq_aarch32(q7_t *input, const uint16_t input_x, const uint16_t input_y,
                const uint16_t input_ch, const q7_t *kernel, const int32_t *bias, const int32_t *biasR,
                const float *scales,
                const int32_t output_offset, const int32_t input_offset,
                const int32_t output_activation_min,
                const int32_t output_activation_max, q7_t *output,
                const uint16_t output_x, const uint16_t output_y,
                const uint16_t output_ch, q15_t *kbuf, q15_t *runtime_buf, q7_t pad_value);

tinyengine_status depthwise_kernel3x3_stride2_inplace_CHW_fpreq_ori(q7_t *input, const uint16_t input_x, const uint16_t input_y,
                const uint16_t input_ch, const q7_t *kernel, const int32_t *bias, const int32_t *biasR,
                const float *scales,
                const int32_t output_offset, const int32_t input_offset,
                const int32_t output_activation_min,
                const int32_t output_activation_max, q7_t *output,
                const uint16_t output_x, const uint16_t output_y,
                const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value);

tinyengine_status depthwise_kernel5x5_stride1_inplace_CHW_fpreq_ori(q7_t *input, const uint16_t input_x, const uint16_t input_y,
                const uint16_t input_ch, const q7_t *kernel, const int32_t *bias, const int32_t *biasR,
                const float *scales,
                const int32_t output_offset, const int32_t input_offset,
                const int32_t output_activation_min,
                const int32_t output_activation_max, q7_t *output,
                const uint16_t output_x, const uint16_t output_y,
                const uint16_t output_ch, q15_t *runtime_buf, q7_t pad_value);

#endif /* TESTOP_H_ */
