## C Development on macOS

### I. compile a first C program on macOS

#### 1.1 compiler installation
gcc/g++, clang/clang++, and llvm are all supported for macOS.

***Xcode Command Line Tools*** is the native tools for macOS, containing its own clang/clang++ compiler inside. It's usually installed together with brew, or it can be installed with `xcode-select --install`. clang/clang++ is installed then (e.g., inside `/Library/Developer/CommandLineTools/usr/bin`)

Then is time for `make` and `cmake`. After both are installed, you are ready to compile.

#### 1.2 compile MNN



### II. cross-compile a C program for iOS
