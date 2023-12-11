/* The part of declaration can be moved to the header. */
#include "hal_data.h"
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

/* The part of declaration can be moved to the header. */

void handle_error_timer (fsp_err_t err){}

uint32_t cmt_free_counter_freq_hz(){
    /* Get the source clock frequency (in Hz). There are several ways to do this in FSP:
     *  - Use the R_CMTW_InfoGet function (it accounts for the clock source and divider).
     *  - Calculate the current PCLKL frequency using R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKL) and right shift
     *    by timer_cfg_t::source_div.
     *
     * This example uses the last option (R_FSP_SystemClockHzGet).
     */
    return R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKL) >> g_timer0_cfg.source_div;
}

void cmt_free_counter_open(){
    /* Initializes the module. */
    fsp_err_t err = R_CMTW_Open(&g_timer0_ctrl, &g_timer0_cfg);
    handle_error_timer(err);

    /* Use the CMTW as a free running counter */
    err = R_CMTW_PeriodSet(&g_timer0_ctrl, 0xFFFFFFFF);
    handle_error_timer(err);
}

void cmt_free_counter_start(){
    R_CMTW_Start(&g_timer0_ctrl);
}

void cmt_free_counter_stop(){
    R_CMTW_Stop(&g_timer0_ctrl);
}

/* return the time interval between start and stop.
 * unit: microsecond (\mu s)
 */
uint32_t cmt_free_counter_get_time(uint32_t timer_freq_hz){
    uint32_t ans = 0;
    timer_status_t status;
    R_CMTW_StatusGet(&g_timer0_ctrl, &status);
    ans = (uint32_t) ((uint64_t) status.counter * TIMER_MIRCOSEC_PER_SEC / timer_freq_hz);
    return ans;
}