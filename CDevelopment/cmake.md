## Cmake
Author: Zhengxiang Huang

examples at "train/2. C-Cpp-Make-Cmake"
reference at https://cmake.org/cmake/help/latest/guide/tutorial/index.html

### 0. Introduction: cmake given CMakeLists.txt
#### 0.1 usual directory structure
```
|- base-dir
    |- build # (you mkdir it)
        |- ... # (your built caches and binaries will reside here)
    |- doc
    |- source
        |- package
            |- ...
        |- .cpp
    |- include
        |- .hpp
    |- CMakeLists.txt
    |- README.md
```

You shall first `mkdir build && cd build` to create a new directory for compilation.

#### 0.2 cmake
You work in the build directory, and pass the `CMakerLists.txt`'s path to the first argument of cmake command.
```bash
cd build
cmake ..
```

You shall also append cmake config definitions, e.g., set up compilation for CUDA. (See also Sec 4.1) Usually, you shall refer to README.md for such project cmake configuations. Vice versa, you shall explain and include such information in your project too.

```bash
# example
cmake .. -DUSE_CUDA=ON
```

make. After cmake, `Makefile` is generated. We then use GNU tool `make` to make.

```bash
# assign a proper number of concurrent jobs
make -j20
```

#### 0.3 make outcomes
You can usually get all your compiled binaries under the `build` directory or any directories you execute your build command. They usually lies plainly under `build`, unless you have added subdirectories.
```
|- base-dir
    |- build # (you mkdir it)
        |- main
        |- libstatic.a
        |- dynamic.so
        |- subdir
            |- subdirlib.so
```
Compiled objects can be categorized into <b>executable</b> and <b>library</b>.
In the example above, `main` is the executable, and `libstatic.a` and `dymanic.so` are libraries, static and dynamic/shared repectively, along with `subdir` being the subdirectory containing a library.

* What are executables?
    only 1 `int main()` exists for 1 executable target. the executable can be directly run from the command line as long as all the dependencies are correctly linked.

* What are libraries? How do static library and dynamic library work?
    Libraries can not be directly executed, as they are binaries storing compiled functions, supplying which for executables. Functions' binary in static library is plugged into the executable binary link-time, while functions' binary in dynamic library remains their and is referenced and executed runtime.
    Dynamic library suffixes `.so` (shared object) on linux and `.dll` (dynamic link library) on windows, while static library suffixes `.a` (archive) on linux and `.lib` (library) on windows. Dynamic library is specified with the keyword `SHARED`, and static library is specified with the keyword `STATIC`.

#### 0.4 remake
1. If no major modification (no adding new files, modifying CMakeLists.txt or something crucial to cmake), just another `make -j20` is okay, CMakeCache can be used.
2. If major modification is conducted, you shall remove everything in the build directory (CMakeCache are outdated) and cmake all over again from very beginning.


### 1. project initialization
#### 1.1 necessities
https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html
https://cmake.org/cmake/help/latest/command/cmake_minimum_required.html
https://cmake.org/cmake/help/latest/command/project.html

2 things are necessary for a cmake file.
1. `cmake_minimum_required()`
2. `project()`

```cmake
# must designate the minimum cmake version
cmake_minimum_required(VERSION 3.10)
# assign the name of the project
project(Try)
```

#### 1.2 set C and Cpp standard
std libraries' api are subject to C and CXX standard.

```cmake
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# Also, append corresponding std standard version to the flags.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
```

* Check your api's standard beforehand.

#### 1.3 Optimization Level and C CXX Flags
Optimization level is specified in the following website:
https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html

You may set `Og` level optimization for debugging, because higher level of optimization may reorder the operations or even skip some of them. `-O3` is the highest to ensure results complete equivalence under IEEE standard.

```cmake
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
```

Even faster optimization is available, along with the cost of multi-threaded race-and-hazards and floating point precision loss.

```cmake
if(FAST_MATH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstrict-aliasing -ffunction-sections -fdata-sections -ffast-math -fno-rtti -fno-exceptions ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -ffunction-sections -fdata-sections -ffast-math")
endif()
```


### 2. header inclusion
https://cmake.org/cmake/help/latest/command/include_directories.html
In your source file, when you want to include a header `header.hpp`, you add it as follows:
```cpp
#include "header.hpp"
```
* How does `#include` work?: 
    The `#include` macro will paste the whole file there. So, be aware the position you paste, usually on top of your file.
* What's the valid suffix of header?
    `.h` is suitable for both c and cpp, while `.hpp` is only for cpp program's header.

To make sure that the cmake can find your header files, you shall include the parent directories of your header files. (`{CMAKE_CURRENT_LIST_DIR}` is the directory path of the current `CMakeLists.txt`, can be the root folder or not)

```cmake
# include header file directories
# `{CMAKE_CURRENT_LIST_DIR}` is the directory path of the current `CMakeLists.txt`, can be the root folder or not
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/
    ${CMAKE_CURRENT_LIST_DIR}/nn/
    ${CMAKE_CURRENT_LIST_DIR}/Tensor/
)
```

### 3. add target, operate files and lists
#### 3.1 target types
In cmake, there are 2 types of target:
executable: https://cmake.org/cmake/help/latest/command/add_executable.html
library: https://cmake.org/cmake/help/latest/command/add_library.html

1. <b>executable</b>: for 1 executable target, only 1 `int main()` exists for 1 executable target. Target usually has no suffix, `.out`, or `.exe` suffix.
    ~~~cmake
    # the EXE_NAME 
    add_executable(${EXE_NAME} ${SRC_FILES})
    # e.g., EXE_NAME being llm_demo, demo.cpp, llm.cpp being all the source.
    add_executable(llm_demo demo.cpp llm.cpp)
    ~~~
    * only 1 `int main()` exists for 1 executable target. the executable can be directly run from the command line as long as all the dependencies are correctly linked.

2. <b>library</b>: Do not consists of `int main()`. Compose a series of source files into 1 library file, either dynamic library or static library. 
    * How do static library and dynamic library work?
    Libraries can not be directly executed, as they are binaries storing compiled functions, supplying which for executables. Functions' binary in static library is plugged into the executable binary link-time, while functions' binary in dynamic library remains their and is referenced and executed runtime.
    Dynamic library suffixes `.so` (shared object) on linux and `.dll` (dynamic link library) on windows, while static library suffixes `.a` (archive) on linux and `.lib` (library) on windows. Dynamic library is specified with the keyword `SHARED`, and static library is specified with the keyword `STATIC`.
    - For a build-from-source library (Normal libraries).
        ```cmake
        add_library(${STATIC_LIB} STATIC ${SRC_FILES}) 
        add_library(${DYNAMIC_LIB} SHARED ${SRC_FILES})
        ```
    - For a imported pre-built library (Imported libraries).
        ```cmake
        add_library(${STATIC_LIB} STATIC IMPORTED [GLOBAL])
        add_library(${DYNAMIC_LIB} SHARED IMPORTED [GLOBAL]) 
        ```
        `Global` specifies whether this library is globally visible or just visible for this directory and below.
        The target property `IMPORTED_LOCATION` shall be set with `set_target_properties()` (as described in Sec 5.2), otherwise `no rules to make ...`error will be raised compile-time.

#### 3.2 Gather source files with `FILE`
https://cmake.org/cmake/help/latest/command/file.html
Specifying the target source files one by one is stupid, and thus we may want to designate the cmake generator to gather all the files under a certain directories to be our source files. Use `FILE(GLOB <out-var> <globbing-expr>)` to glob the file names to the out-var.

```cmake
# FILE(GLOB <out-var> <globbing-expr>)
FILE(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
```
After the above command, the files matched in `${CMAKE_CURRENT_LIST_DIR}/src/*.cpp` are set to `SRC_FILES`.

* Every time you add a new file, you need to cmake again to re-glob the files, because globbing happens at cmake-time.
* C/C++ source files usually suffix `.c`, `.cpp`, `.cc`, and `.cxx`.

#### 3.3 Maintain and Iterate a list of files/objects
https://cmake.org/cmake/help/latest/command/list.html

```cmake
list(APPEND MNN_TARGETS MNN)
list(REMOVE_ITEM MNN_TARGETS MNN)
```

Use `foreach()` and `endforeach()` to iterate through a list of items.
```cmake
    foreach(TARGET ${MNN_TARGETS})
      target_compile_definitions(${TARGET} PRIVATE "-DBUILDING_MNN_DLL")
      target_compile_definitions(${TARGET} INTERFACE "-DUSING_MNN_DLL")
    endforeach()
```

#### 3.4 add a subdirectory
You may want to put some of your source files under some subdirectories instead of stuffing all the files under the main directory.

```cmake
add_subdirectory(source_dir [binary_dir])
```

`source_dir` specifies where your subdirectory is. subdirectory needs to contain a `CMakeLists.txt` itself.

You can specify a designated `binary_dir` so that the targets in subdirectory will be built in your specified location.


### 4. options, conditional clauses, compile definitions
When you what to compile your programs for multiple use cases, especially for multiple backends, setting options and cmake according to conditional clauses is essential.

#### 4.1 cmake option
```cmake
# option(<variable> "<help_text>" [default-value])
# e.g.
option(USE_CUDA "enable CUDA or not, default OFF" OFF)
```
Compile users can configure the `<variable>` by switching `ON` and `OFF` when camke in the terminal. The example command is as follows:

```bash
# turn on USE_CUDA 
cmake .. -DUSE_CUDA=ON
```

* Add `-D` prefix before the definition. e.g., assigning variable named after `USE_CUDA` requires appending `-DUSE_CUDA=ON` to the command.

#### 4.2 basic conditional clauses
After the option is set, we want it to control our cmake workflow. if-[else/elseif]-endif clause (`else()` and `elseif()` is optional, but `endif()` must be closed) enables such control.


```cmake
# if-[else]-endif clause.
if()
    # ...
else()
    # ..
endif()

```

The logical calculation is enabled by `NOT`, `AND`, and `OR`. String comparison is enabled by `MATCHES`.

```cmake
if((NOT VAR1 AND NOT VAR2) OR VAR3)
    message("HERE")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug Mode")
endif()
```

#### 4.3 compile definitions
Usually, inside the conditional clauses, we can instruct to compile or link different objects in different subclauses. However, we may also want to set some definitions (<b>macro</b>), so that compilation workflow inside the files can also be connected to the cmake process.
In source files you may write 
```cpp
#ifdef DEBUG
printf("abc: %d", abd);
#endif
```
To add the definition of the macro `DEBUG`, you shall add it in the CMakeLists.txt.

`add_compile_definitions()` and its superordinate superseded alternative `add_definitions()` can add compile definitions globally, with `target_compile_definitions()` for a specific target.
https://cmake.org/cmake/help/latest/command/add_definitions.html
https://cmake.org/cmake/help/latest/command/add_compile_definitions.html
https://cmake.org/cmake/help/latest/command/target_compile_definitions.html

Preprocessor definitions are flags beginning in `-D` or `/D`.

```cmake
# with value assignment
add_definitions(-DCOREML_ENABLED=1)
# no value
add_definitions(-DSUPPORT_TRANSFORMER_FUSE)
# new api
add_compile_definitions(-DSUPPORT_TRANSFORMER)
# for a specific target
target_compile_definitions(CPU PRIVATE "-DUSE_ONEDNN")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(CPU PRIVATE "_DDEBUG")
endif()
```
Then, these macros can be directly accessed in your code!

### 5. set target properties
All the keywords setting target properties follow the following pattern:
`target_include_directories(<target> <PUBLIC|PRIVATE> <args>)`

#### 5.1 target include directories
https://cmake.org/cmake/help/latest/command/target_include_directories.html

You can individually add directories to a specific target (executable or library) with `target_include_directories`.
```cmake
target_include_directories(<target> <PUBLIC|PRIVATE> <dir>)
```
This directory is not visible to other objects.

#### 5.2 set_target_properties()
https://cmake.org/cmake/help/latest/command/set_target_properties.html
syntax:
```cmake
set_target_properties(<targets> ...
                      PROPERTIES <prop1> <value1>
                      [<prop2> <value2>] ...)
```
For an imported link library, as aforementioned in Sec 3.2, you shall specify its location, which can be set with `set_target_properties()`.

```cmake
    set_target_properties(
        MNN
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_LIST_DIR}/MNN-2_9_5/libs/phone/libMNN.so
        )
```

#### 5.3 target link libraries
https://cmake.org/cmake/help/latest/command/target_link_libraries.html
Among all usages, 3 of them are frequently-used, 
1. link a library target,
2. link a full path to the library, 
3. link a library name.
In the following example, `Express` is 1, `libMNN.so` is 2, `log m` is 3.
```cmake
add_library(Express ${EXPR_SRC})
add_library(Model ${SRC_FILES})
# Express is 1, libMNN.so is 2, log m is 3
target_link_libraries(Model
                        PUBLIC 
                        Express 
                        ${CMAKE_CURRENT_LIST_DIR}/lib/libMNN.so
                        log m)
```

### 6. find external package/library
#### 6.1 find library
https://cmake.org/cmake/help/latest/command/find_library.html
```cmake
find_library (
          <VAR>
          name | NAMES name1 [name2 ...] [NAMES_PER_DIR]
          [HINTS [path | ENV var]... ]
          [PATHS [path | ENV var]... ]
          [REGISTRY_VIEW (64|32|64_32|32_64|HOST|TARGET|BOTH)]
          [REQUIRED]
         )
# example
find_library(Metal Metal REQUIRED)
```
<VAR> being the var you want to store the found lib, name being the name of that lib, `REQUIRED` specify whether the lib is required. If not found, result will be set to `<VAR>-NOTFOUND`.

The found library shall be linked in the following.

#### 6.2 find package
Unlike finding a library, `find_package` can find and load not only all the libraries but also the detailed information also.

```cmake
find_package(<PackageName> [<version>] [REQUIRED])
# example: find OpenMP and then add the libraries in it.
find_package(OpenMP)
add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
```

After you find the package, then every corresponding information can be accessed.

### 7. Mixed Compilation (C, Cpp, Asm, Cuda...)

#### 7.1 Compile Cuda



### 8. Cross Compilation (compile for another machine)



### 9. Exercises and Examples


