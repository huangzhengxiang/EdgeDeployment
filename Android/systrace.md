## systrace & Perfetto

### 1. Performance tracing

#### 1.1 On device tracing
https://developer.android.com/topic/performance/tracing/on-device

#### 1.2 cmd tracing
https://perfetto.dev/docs/quickstart/android-tracing

```bash
adb shell setprop persist.traced.enable 1
curl -O https://raw.githubusercontent.com/google/perfetto/main/tools/record_android_trace
chmod u+x record_android_trace
./record_android_trace -o trace_file.perfetto-trace -t 30s -b 64mb sched freq idle am wm gfx view binder_driver hal dalvik camera input res memory
```


#### 1.3 Perfetto visualization
https://ui.perfetto.dev/
Used to visualize CPU utilization.https://developer.android.com/topic/performance/power/setup-battery-historian


#### 1.4 Android studio standalone profiler
https://developer.android.com/studio/profile/standalone-profiler
initiate the profiler in the installation path bin.

### 2. Battery Profiling

#### 2.1 battery historian (go/docker problem)
https://developer.android.com/topic/performance/power/setup-battery-historian 
https://github.com/google/battery-historian

Use `BatteryManager` to record battery in java (Android Studio app). 
https://developer.android.com/reference/android/os/BatteryManager.html#BATTERY_PROPERTY_CURRENT_NOW


#### 2.2 ACTION_BATTERY_CHANGED (too coarse)
`EXTRA_LEVEL`, `EXTRA_SCALES`, and `CAPACITY_LEVEL` are all of the granularity of integer part of 100%, no fractional part. Too coarse for energy analysis.

#### 2.3 AlarmManager (interval at least 1 min)
If `AlarmManager` (which is also a service handled by OS) is used, it will check that battery level every user-defined (appropriate time). However, `AlarmManager`'s time inteval is at least 1 minute.


#### 2.4 Timer & TimerTask (appropriate at least for now)
The best way for energy profiling DNN inference energy consumption is to use `TimerTask`.

In EnergyMonitor.java
```java
// EnergyMonitor.java
public class EnergyMonitor extends TimerTask {
    private final ArrayList<Integer> mAList;
    private final Context ctx;

    EnergyMonitor(Context parent) {
        mAList = new ArrayList<Integer>();
        ctx = parent;
    }

    public void resetInfo() {
        mAList.clear();
    }

    public int getAvgCurrent() {
        long size = mAList.size();
        long res = 0;
        for (int i=0; i < size; ++i) {
            res += mAList.get(i);
        }
        return  (int)(res/size);
    }

    @Override
    public void run() {
        int mBatteryCurrent = ((BatteryManager) ctx.getSystemService(Context.BATTERY_SERVICE)).getIntProperty(BatteryManager.BATTERY_PROPERTY_CURRENT_NOW);
        mAList.add(mBatteryCurrent);
    }
}
```

In MainActivity.java, initialize the Timer and TimerTask.
```java
// MainActivity.java
private void startEnergyTracing() {
    energyMonitor.resetInfo();
    energyTimer.scheduleAtFixedRate(energyMonitor, 0, energySampleInterval);
}

private void endEnergyTracing() {
    energyTimer.cancel();
}

private int getAvgCurrent() {
    return energyMonitor.getAvgCurrent();
}

// in onCreate
energyTimer = new Timer();
energyMonitor = new EnergyMonitor(this);
startEnergyTracing();
```