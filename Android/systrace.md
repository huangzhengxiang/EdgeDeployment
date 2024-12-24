## systrace & Perfetto

### On device tracing
https://developer.android.com/topic/performance/tracing/on-device

### cmd tracing
https://perfetto.dev/docs/quickstart/android-tracing

```bash
adb shell setprop persist.traced.enable 1
curl -O https://raw.githubusercontent.com/google/perfetto/main/tools/record_android_trace
chmod u+x record_android_trace
./record_android_trace -o trace_file.perfetto-trace -t 30s -b 64mb sched freq idle am wm gfx view binder_driver hal dalvik camera input res memory
```


### Perfetto
https://ui.perfetto.dev/
Used to visualize CPU utilization.https://developer.android.com/topic/performance/power/setup-battery-historian


### Android studio standalone profiler
https://developer.android.com/studio/profile/standalone-profiler
initiate the profiler in the installation path bin.

### Battery Level Profiling
https://developer.android.com/topic/performance/power/setup-battery-historian 
https://github.com/google/battery-historian

Use `BatteryManager` to record battery in java (Android Studio app). 
https://developer.android.com/reference/android/os/BatteryManager.html#BATTERY_PROPERTY_CURRENT_NOW