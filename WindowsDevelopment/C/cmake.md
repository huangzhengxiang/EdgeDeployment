## 0. Installation
https://cmake.org/download/


## 1. Generator
The default generator on Windows is `NMake Makefiles`, which can be installed with VS.

The Posix style makefile can be set with `-G`.
```bat
cmake .. -G "Unix Makefiles"
```

You can add a new global environment variable `CMAKE_GENERATOR` with the value of `Unix Makefiles` to change the default.

Check it with `echo`.
```bat
echo %CMAKE_GENERATOR%
```