## Debug native C program with lldb

If you built your native program with Clang/LLVM, then LLDB is the right debugger. On Android NDK, lldb is the modern replacement for gdb.if you built with Clang/LLVM, then LLDB is the right debugger. On Android NDK, lldb is the modern replacement for gdb.


### 1. Find lldb-server and push to device
It’s inside your NDK, something like: 
```bash
$ANDROID_NDK/toolchains/llvm/prebuilt/linux-x86_64/lib/clang/18/lib/linux/aarch64/lldb-server
```

Then, push to your phone directory:
```bash
adb push $ANDROID_NDK/toolchains/llvm/prebuilt/linux-x86_64/lib/clang/18/lib/linux/aarch64/lldb-server /data/local/tmp/llm/
adb shell chmod 777 /data/local/tmp/llm/lldb-server
```

### 2. On device: Launch program and bind to a port

Use port 5039 as an example:
```bash
/data/local/tmp/lldb-server g :5039 -- <program-exe> <args>
```

### 3. On host: Connect to the remote port and start debugging

Forward the adb port and start lldb service:
```bash
# Forward the port
adb forward tcp:5039 tcp:5039
# start lldb
lldb
```

Inside lldb, start debugging:
```bash
# inside lldb
platform select remote-android
target create /path/to/local/rag_demo   # your local build binary
gdb-remote 127.0.0.1:5039
```
Notice that the target is the same executable file that you pushed to the phone!

### 4. Simple lldb commands
1. continue
At the beginning, it pauses at the begin of the program. `continue` it.
```bash
# inside lldb
continue
```

2. bt
When your program crash, you can locate it by backtracing `bt`.
```bash
# inside lldb
bt
```

You would see stack like this, if you've exported symbols, you can also see them here:
```
* thread #1, name = 'rag_demo', stop reason = signal SIGSEGV: address not mapped to object (fault address: 0x0) * frame #0: 0x0000007d57789544 frame #1: 0x0000007d57769730 frame #2: 0x0000007d57745728 frame #3: 0x0000007d591d22fc frame #4: 0x0000007d591d1d04 frame #5: 0x0000007d59158bbc frame #6: 0x0000007d5915adac frame #7: 0x0000007d59127f68 frame #8: 0x0000007d59193118 frame #9: 0x0000007d59193ba8 frame #10: 0x0000007d59251e54 frame #11: 0x0000007d5923846c frame #12: 0x0000007d59229d74 frame #13: 0x0000007d5922407c frame #14: 0x0000007d5922b4b8 frame #15: 0x0000007fef360e30 frame #16: 0x0000007fef365ea4 frame #17: 0x0000007fef359b98 frame #18: 0x00000055555591e0 rag_demo_start_main + 88
```

You can then locate each lines' library/exe with `image list`.
```bash
# inside lldb
image list
```
See lines like this:
```
[ 32] 9911CA5B /system/lib64/libvndksupport.so (0x0000007d593c5000)
```