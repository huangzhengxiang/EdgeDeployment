/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   main.cpp
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "testing_data/golden_data.h"
#include "testing_data/images.h"
#include "genNN.h"
#include "tinyengine_function.h"



#define RES_W 80
#define RES_H 80
#define OUTPUT_CH 2

signed char out_int[OUTPUT_CH];

float labels[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

void invoke_new_weights_givenimg(signed char *out_int8) {
  invoke(labels);
  signed char *output = (signed char *)getOutput();
  for (int i = 0; i < OUTPUT_CH; i++)
    out_int8[i] = output[i];
}


int hal_entry(void)
{
    signed char *input = getInput();
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = person[i];  // Image of person
    }
    // Check if image of person is predicted as a person.
    invoke_new_weights_givenimg(out_int);
    int isPerson = 0;
    if (out_int[0] > out_int[1]) {
        isPerson = 0;
        // printf("Not a person.\n");
    }
    else {
        isPerson = 1;
        // printf("This is a person.\n");
    }

    // Another test.
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = no_person[i];  // Image of non-person
    }
    // Check if image of non-person is predicted as a non-person.
    invoke_new_weights_givenimg(out_int);
    if (out_int[0] > out_int[1]) {
        isPerson = 0;
        // printf("Not a person.\n");
    }
    else {
        isPerson = 1;
        // printf("This is a person.\n");
    }
    return 0;
}
