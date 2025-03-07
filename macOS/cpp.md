## C Development on macOS

### I. compile a first C program on macOS

#### 1.1 compiler installation
gcc/g++, clang/clang++, and llvm are all supported for macOS.

***Xcode Command Line Tools*** is the native tools for macOS, containing its own clang/clang++ compiler inside. It's usually installed together with brew, or it can be installed with `xcode-select --install`. clang/clang++ is installed then (e.g., inside `/Library/Developer/CommandLineTools/usr/bin`)

Then is time for `make` and `cmake`. After both are installed, you are ready to compile.


***Full Xcode*** can be installed either from APP Store or its official website.
Lateset Xcode version: https://developer.apple.com/download/applications/


***Switch between above two***
https://github.com/nodejs/node-gyp/issues/569
Note that `xcodebuild` can only be used with full Xcode, so that before compiling with xcodebuild command, full version Xcode shall be selected. (Change it to your own path.)

```zsh
# Change the path if you installed Xcode somewhere else.
sudo xcode-select -s /path/to/Xcode.app/Contents/Developer
```

You can try to switch back to command lin tool if you wish.
```zsh
sudo xcode-select --switch /Library/Developer/CommandLineTools # Enable command line tools
```

***Xcode Proxy*** can directly been set by setting it in the zsh shell and opening the project from the shell. 
(refer to https://forums.swift.org/t/xcode-not-follows-system-proxy-settings-to-resolve-swiftpm-dependencies/34137/4)
This way of setting proxy is graceful indeed.

#### 1.2 compile MNN



### II. cross-compile a C program for iOS
