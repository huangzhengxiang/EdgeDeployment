/*
 * timing.h
 *
 *  Created on: 2023年10月18日
 *      Author: hzx
 */

#ifndef TIMING_H_
#define TIMING_H_

#include "hal_data.h"

void invoke_time(float* labels);

#define TIMER_MIRCOSEC_PER_SEC 1000000

void handle_error_timer (fsp_err_t err);

/* A free running execution timer getter.
 * procedure: construction, open, start, stop, get_time.
 */
uint32_t cmt_free_counter_freq_hz();
void cmt_free_counter_open();
void cmt_free_counter_start();
void cmt_free_counter_stop();
uint32_t cmt_free_counter_get_time(uint32_t timer_freq_hz);

#endif /* TIMING_H_ */
