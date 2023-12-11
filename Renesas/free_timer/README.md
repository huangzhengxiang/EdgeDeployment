## Configuration for free_timer API (CMTW)

Configurations that shall be made:

- <b>configuration.xml</b>: add `Timer Driver on r_cmtw` in `New Stacks`, `Driver`, `Timers`, and then press `Generate Project Content`.

- <b>C/C++ Settings</b>: None.

- <b>code copying</b>: copy `hal_entry.c`'s content into your own `hal_entry.c`.

- <b>setup steps</b>: see function calls.

- Finally, build your code and debug it!


## function calls
- <b>cmt_free_counter_freq_hz</b>: return the frequency of the timer. (Unit: Hz)

- <b>cmt_free_counter_open</b>: first <b>reset</b> and then open the timer.

- <b>cmt_free_counter_start</b>: start counting.

- <b>cmt_free_counter_stop</b>: stop counting, and the counter value is ready to read.

- <b>cmt_free_counter_get_time</b>: given the frequency, return the time interval. (Unit: microseconds ($\mu s$))

## example code
~~~
cmt_free_counter_open();
cmt_free_counter_start();
/* Your function goes here! */
myfunc();
/* *********************** */
cmt_free_counter_stop();
uint32_t baseline_exe_time = cmt_free_counter_get_time(cmt_free_counter_freq_hz());
~~~