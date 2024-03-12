## Configuration for sci_uart API

Configurations that shall be made:

- <b>configuration.xml</b>: add `UART Driver on r_sci_uart` in `New Stacks`, `Driver`, `Connectivity`, and then press `Generate Project Content`.

- <b>C/C++ Settings</b>:  uncheck `Use newlib-nano (--specs=nano.specs)` and check `Do not use syscalls (--specs=nosys.specs)` in `Miscellaneous` in `Cross ARM C Linker` in `C/C++ Settings` (<b>This one is crucial!!!</b>), so that our own `_write` and `_read` will be enabled.

- <b>code copying</b>: put files in this directory, other than `hal_entry.c`, into `src`, and copy `hal_entry.c`'s content into your own `hal_entry.c`.

- <b>setup steps</b>: add a call to `setup_sci_uart()` in your `hal_entry()` function, so that you are ready to use `printf()` in your code and communicate use your computer via serial port.

- Finally, build your code and debug it!


## function calls
- <b>func</b>: stable!
