# Timer

## url
https://software-dl.ti.com/mcu-plus-sdk/esd/AM243X/latest/exports/docs/api_guide_am243x/KERNEL_DPL_TIMER_PAGE.html
https://software-dl.ti.com/mcu-plus-sdk/esd/AM243X/latest/exports/docs/api_guide_am243x/KERNEL_DPL_CLOCK_PAGE.html

## ${ProjDirPath}/Debug/syscfg folder
This folder consists of critical auto generated files which can be applied to manipulate Hardware.

## example code
~~~
#include <kernel/dpl/ClockP.h>

/* start the tick timer */
uint64_t start, end;
start = ClockP_getTimeUsec();
/* Your function goes here! */
myfunc();
/* *********************** */
/* stop the tick timer */
end = ClockP_getTimeUsec();
uint32_t time_val = end - start;

/* calculate fps also */
float fps = 1000000.f / time_val;

/* print time, "%ld" and "%u" both acceptable to format uint32_t. */
DebugP_log("execution time: %ld mus, fps: %.2f \n", time_val, fps);
~~~