#include "hal_data.h"
#include "stdio.h"
#include "string.h"
#include "testing_data/golden_data.h"
#include "testing_data/images.h"
#include "testing_data/vww_test.h"
#include "genNN.h"
#include "tinyengine_function.h"

#include "sio_char.h"
#include <stdio.h>

#include "testOp.h"
#include "genModel.h"

#include "timing.h"

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER
void handle_error_sci_uart (fsp_err_t err);
void user_uart_callback (uart_callback_args_t * p_args);
void r_sci_uart_baud_example (void);

uint8_t  g_out_of_band_received[TRANSFER_LENGTH];
volatile uint32_t g_transfer_complete = 0;
volatile uint32_t g_receive_complete  = 0;
uint32_t g_out_of_band_index = 0;

void setup_sci_uart (void);

void r_sci_uart_baud_example (void)
{
    baud_setting_t baud_setting;
    uint32_t       baud_rate                 = SCI_UART_BAUDRATE_19200;
    bool           enable_bitrate_modulation = false;
    uint32_t       error_rate_x_1000         = 5000;
    fsp_err_t err = R_SCI_UART_BaudCalculate(baud_rate, enable_bitrate_modulation, error_rate_x_1000, &baud_setting);
    handle_error_sci_uart(err);
    err = R_SCI_UART_BaudSet(&g_uart0_ctrl, (void *) &baud_setting);
    handle_error_sci_uart(err);
}

void user_uart_callback (uart_callback_args_t * p_args)
{
    /* Handle the UART event */
    switch (p_args->event)
    {
        /* Received a character */
        case UART_EVENT_RX_CHAR:
        {
            /* Only put the next character in the receive buffer if there is space for it */
            if (sizeof(g_out_of_band_received) > g_out_of_band_index)
            {
                /* Write either the next one or two bytes depending on the receive data size */
                if (UART_DATA_BITS_8 >= g_uart0_cfg.data_bits)
                {
                    g_out_of_band_received[g_out_of_band_index++] = (uint8_t) p_args->data;
                }
                else
                {
                    uint16_t * p_dest = (uint16_t *) &g_out_of_band_received[g_out_of_band_index];
                    *p_dest              = (uint16_t) p_args->data;
                    g_out_of_band_index += 2;
                }
            }
            break;
        }
        /* Receive complete */
        case UART_EVENT_RX_COMPLETE:
        {
            g_receive_complete = 1;
            break;
        }
        /* Transmit complete */
        case UART_EVENT_TX_COMPLETE:
        {
            g_transfer_complete = 1;
            break;
        }
        default:
        {
        }
    }
}


void handle_error_sci_uart (fsp_err_t err)
{
    FSP_PARAMETER_NOT_USED(err);
}

/*******************************************************************************************************************//**
 * @brief  SCI_UART example application
 *
 * Sample program for serial communication.
 * After outputting Hello world, the received character is echoed back.
 *
 **********************************************************************************************************************/
void setup_sci_uart (void)
{
    /* Open the transfer instance with initial configuration. */
    fsp_err_t err = R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    handle_error_sci_uart(err);
    r_sci_uart_baud_example();

    __asm volatile ("cpsie i");

}

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


#ifdef mbv2
#define RES_W 144
#define RES_H 144
#define OUTPUT_CH 1000
#else
#define RES_W 80
#define RES_H 80
#define OUTPUT_CH 2
#endif

signed char out_int[OUTPUT_CH];

float labels[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

void invoke_new_weights_givenimg(signed char *out_int8) {
  invoke(labels);
  signed char *output = (signed char *)getOutput();
  for (int i = 0; i < OUTPUT_CH; i++)
    out_int8[i] = output[i];
}

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event)
BSP_PLACE_IN_SECTION(".warm_start");
FSP_CPP_FOOTER

#ifdef TESTOP
// Open some more buffer here!
int16_t sbuf_test[8000];
int16_t kbuf_test[2000];
int8_t result_test[25600];
#endif

#ifdef UnitTest
#define Test_H 4
#define Test_W 4
#define Test_H_OUT 4
#define Test_W_OUT 4
#define Test_inputch 4
#define Test_outputch 4
//q7_t kernel_test_ori[Test_outputch*1*1*Test_inputch]; // (c_out=8, 1, 1, c_in=16)
//q7_t kernel_test_transposed[Test_inputch*1*1*Test_outputch]; // (c_out=8, 1, 1, c_in=16)
//q7_t img_buffer[Test_H*Test_W*Test_inputch]; // (40,40,16)
//q7_t ours_output[Test_H*Test_W*Test_outputch]; // (40,40,8)
//q7_t theirs_output[Test_H*Test_W*Test_outputch]; // (40,40,8)

q7_t kernel_test_ori[Test_outputch*3*3]; // (c_out=8, 1, 1, c_in=16)
//q7_t kernel_test_transposed[Test_inputch*1*1*Test_outputch]; // (c_out=8, 1, 1, c_in=16)
q7_t img_buffer[Test_inputch*Test_H*Test_W]; // (40,40,16)
q7_t ours_output[Test_outputch*Test_H*Test_W]; // (40,40,8)
q7_t theirs_output[Test_outputch*Test_H*Test_W]; // (40,40,8)

float scales_test[Test_outputch];
int32_t bias_test[Test_outputch];
const int32_t in_offset=-2;
const int32_t out_offset=7;
// Open some more buffer here!
int16_t sbuf_test[16000];
int16_t kbuf_test[2000];
#endif

void invoke_multi_test(){
    int succ_time = 0;
    for (int t = 0; t< TestNum; ++t){
        int8_t* input = getInput();
        for (int i = 0; i < RES_W * RES_W * 3; i++) {
            input[i] = test_data[t*(RES_W * RES_W * 3)+i];  // Image
        }
        cmt_free_counter_open();
        cmt_free_counter_start();
        invoke_new_weights_givenimg(out_int);
        cmt_free_counter_stop();
        uint32_t inference_exe_time = cmt_free_counter_get_time(cmt_free_counter_freq_hz());
        int isPerson = 0;
//        printf("%d, %d\n", out_int[0], out_int[1]);
        if (out_int[0] > out_int[1]) {
            isPerson = 0;
            printf("Not a person. ");
        }
        else {
            isPerson = 1;
            printf("This is a person. ");
        }
        if (isPerson==test_labels[t]){
            succ_time++;
            printf("Succeeded!   ");
        }else{
            printf("Failed!   ");
        }
        printf("%.3f fps\n",(float)TIMER_MIRCOSEC_PER_SEC/inference_exe_time);
    }
    printf("The success rate is: %d/%d.\n", succ_time, TestNum);
}


/*******************************************************************************************************************//**
 * main() is generated by the FSP Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{
    /* TODO: add your own code here */

    setup_sci_uart();

    printf("\n\nA new program starts!\n");

    signed char *input = getInput();
#ifndef mbv2
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = person[i];  // Image of person
    }
#else
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = i % 29 - 11;  // Image of person
    }
#endif
    // Check if image of person is predicted as a person.
    cmt_free_counter_open();
    cmt_free_counter_start();
    invoke_new_weights_givenimg(out_int);
    cmt_free_counter_stop();
    uint32_t inference_exe_time = cmt_free_counter_get_time(cmt_free_counter_freq_hz());
//    printf("The first inference finished in %ld microsecs.\n", inference_exe_time);
#ifndef mbv2
    int isPerson = 0;
    printf("%d, %d\n", out_int[0], out_int[1]);
    if (out_int[0] > out_int[1]) {
        isPerson = 0;
        printf("Not a person.\n");
    }
    else {
        isPerson = 1;
        printf("This is a person.\n");
    }
#endif
    // Another test.
#ifndef mbv2
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = no_person[i];  // Image of non-person
    }
#else
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = i % 29 - 11;  // Image of person
    }
#endif
    // Check if image of non-person is predicted as a non-person.
    invoke_new_weights_givenimg(out_int);

#ifndef mbv2
    printf("%d, %d\n", out_int[0], out_int[1]);
    if (out_int[0] > out_int[1]) {
        isPerson = 0;
        printf("Not a person.\n");
    }
    else {
        isPerson = 1;
        printf("This is a person.\n");
    }
#endif

    // Time Test.
#ifndef mbv2
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = no_person[i];  // Image of non-person
    }
#else
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = i % 29 - 11;  // Image of person
    }
#endif
#ifdef TimeTest
    /* measure the time of each layer. */
    printf("\nStart timing for each layer of MCUNet!\n");
    invoke_time(NULL);
#endif

#ifndef mbv2
    printf("\nStart 10 Tests!\n\n");
    invoke_multi_test();
#endif

#ifdef TESTOP
    printf("\nStart testing new Op!\n");
    input = getInput();
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = person[i];  // Image of person
    }
    cmt_free_counter_open();
    cmt_free_counter_start();
    convolve_s8_kernel3_inputch3_stride2_pad1_fpreq(&buffer0[25600],80,80,3,(const q7_t*) weight0,bias0,scales0,-128,1,-128,127,&buffer0[0],40,40,16,sbuf,kbuf,-1);
    cmt_free_counter_stop();
    uint32_t baseline_exe_time = cmt_free_counter_get_time(cmt_free_counter_freq_hz());
    printf("The baseline running time of first layer is %ld microsecs.\n", baseline_exe_time);

    input = getInput();
    for (int i = 0; i < RES_W * RES_W * 3; i++) {
        input[i] = person[i];  // Image of person
    }
    cmt_free_counter_open();
    cmt_free_counter_start();
    convolve_generic_fpreq(&buffer0[25600],80,80,3,(const q7_t*) weight0_transposed,bias0,scales0,-128,1,-128,127,result_test,40,40,16,sbuf_test,kbuf_test,3,3,2,1,-1);
    cmt_free_counter_stop();
    uint32_t fast_exe_time = cmt_free_counter_get_time(cmt_free_counter_freq_hz());
    printf("The armv8r-aarch32 accelerated running time of first layer is %ld microsecs.\n", fast_exe_time);
    int isCorrect = tensor_comp_no_map(&buffer0[0], result_test, 16*4);
    if (isCorrect == 1){
        printf("The fast algorithm is correct!\n");
    }else{
        printf("Oops! The fast algorithm fails!\n");
    }
#endif
#ifdef UnitTest
    /* measure the time of each layer. */
    invoke_time(NULL);

//    for (int i = 0; i < Test_outputch*1*1*Test_inputch; i++) {
//        kernel_test_ori[i] = i % 30 -15;
//    }
//    for (int i = 0; i < Test_outputch; i++) {
//        for (int j = 0; j < Test_inputch; j++) {
//            kernel_test_transposed[j*Test_outputch+i] = kernel_test_ori[i*Test_inputch+j];
//        }
//    }
//    for (int i = 0; i < Test_H*Test_W*Test_inputch; i++) {
//        img_buffer[i] = i % 30 - 15;
//    }

    for (int i = 0; i < Test_outputch*3*3; i++) {
        kernel_test_ori[i] = i % 30 -15;
    }
    for (int i = 0; i < Test_outputch*Test_H*Test_W; i++) {
        ours_output[i] = i % 30 - 15;
        theirs_output[i] = i % 30 - 15;
    }

    for (int i = 0; i < Test_outputch; i++) {
        scales_test[i] = i * 0.01;
    }
    for (int i = 0; i < Test_outputch; i++) {
        bias_test[i] = i - 4;
    }

    q7_t* ours = ours_output;
    q7_t* theirs = theirs_output;

    cmt_free_counter_open();
    cmt_free_counter_start();
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq(theirs,
            Test_W, Test_H, Test_inputch,
            kernel_test_ori, bias_test, bias_test,scales_test,
            out_offset, in_offset,
            -127, 78,
            theirs, Test_W_OUT, Test_H_OUT,
            Test_outputch, sbuf_test, kbuf_test, 0);
    cmt_free_counter_stop();
    uint32_t baseline_exe_time = cmt_free_counter_get_time(cmt_free_counter_freq_hz());
    printf("The baseline running time of first layer is %ld microsecs.\n", baseline_exe_time);

    cmt_free_counter_open();
    cmt_free_counter_start();
    depthwise_kernel5x5_stride1_inplace_CHW_fpreq_ori(ours,
            Test_W, Test_H, Test_inputch,
            kernel_test_ori, bias_test, bias_test,scales_test,
            out_offset, in_offset,
            -127, 78,
            ours, Test_W_OUT, Test_H_OUT,
            Test_outputch, sbuf_test, 0);
    cmt_free_counter_stop();
    uint32_t fast_exe_time = cmt_free_counter_get_time(cmt_free_counter_freq_hz());
    printf("The armv8r-aarch32 accelerated running time of first layer is %ld microsecs.\n", fast_exe_time);

    int isCorrect = tensor_comp_no_map(ours_output, theirs_output, Test_H_OUT * Test_W_OUT * Test_outputch);
    if (isCorrect == 1){
        printf("The fast algorithm is correct!\n");
    }else{
        printf("Oops! The fast algorithm fails!\n");
    }
#endif

    printf("\n\n");
    return;
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
        /* Pre clock initialization */
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}
