## Android App

### 0. Guide
https://developer.android.com/guide

### 1. gradle, jvm, android gradle dependencies
1. Download required-version gradle from https://services.gradle.org/distributions/
2. Download required-compatible-version jdk from Oracle or corretto.

3. Configure gradle and jdk in `File` -> `Settings` -> `Build, Execution, Deployment` -> `Build Tools` -> `Gradle` in Android Studio.

4. Configure android gradle plugin in `build.gradle` in the project root directory.

#### 1.1 dependencies specifications
1. dependencies of package plugins
2. dependencies of sdk (API level) version <-> ndk version (if native c/cpp is used)
3. dependencies of gradle version (build tools) <-> jdk version (JVM compiler)

#### 1.2 gradle script syntax (Groovy v.s. Kotlin)
https://developer.android.com/build/migrate-to-kotlin-dsl?hl=zh-cn
https://docs.gradle.org/current/userguide/migrating_from_groovy_to_kotlin_dsl.html

### 2. jni

#### 2.1 compilation of jni
确保native方法的声明和实现匹配：
确保Java类中声明的native方法与C/C++代码中的实现完全匹配，包括方法名、参数类型和顺序、返回类型等。JNI接口命名规则是Java_完整类名_方法名_参数类型，例如：Java_com_mnn_llm_Chat_Init。

检查.so库文件是否正确加载：
确保你的.so库文件已经被正确地放置在项目的jniLibs目录下，并且你的build.gradle文件已经配置了对应的路径。如果你将.so文件放在了libs目录下，你需要在build.gradle中添加如下配置：

gradle
sourceSets {
    main {
        jniLibs.srcDirs = ['libs']
    }
}
这样Android Studio在构建时会将libs目录下的.so文件复制到正确的位置。

检查包名和类名是否正确：
错误信息中的包名和类名必须与Java代码中的完全一致。如果更改了Java代码中的包名或类名，需要确保更新JNI代码中的相应部分以保持一致。

确保System.loadLibrary被正确调用：
在你的Java代码中，确保System.loadLibrary方法被调用，并且传递的库名与你的.so文件名（不包括lib前缀和.so后缀）相匹配。例如，如果你的.so文件名为libmnnllm.so，则应在Java代码中调用System.loadLibrary("mnnllm")。所有依赖包都应该被load进来

检查ABI兼容性：
确保你的.so库文件与目标设备的CPU架构兼容。例如，如果你的设备是arm64-v8a架构，那么你需要有对应架构的.so文件。你可以在build.gradle中设置abiFilters来指定目标架构：

gradle
android {
    ndk {
        abiFilters 'armeabi-v7a', 'arm64-v8a'
    }
}

Use keyword `extrern "C"` to wrap your native source code, so that the java compiler won't ppbe confused with the mangling issue.
https://stackoverflow.com/questions/33878523/getting-unsatisfied-link-error-while-making-a-jni-call

![jni dismanled](jni.png)

#### 2.2 type conversion of jni
https://blog.csdn.net/tantion/article/details/84248654
Basic jobject can be convert to c object directly, while jstring and char* may be a little bit trickier (use `env->GetStringUTFChars`).

### 3. LogCat
#### C/C++
由于在Android中不能直接使用cout来输出日志，需要使用Android的日志系统。在JNI代码中包含<android/log.h>头文件，并使用__android_log_print函数来输出日志

#### Java
import android.util.Log;
Log.e([Tag], [msg])
Log.d([Tag], [msg])
Log.i([Tag], [msg])

#### log library linker flag for static compilation
https://github.com/catchorg/Catch2/issues/1720
Add `-DCMAKE_CXX_FLAGS="-llog"` to cmake list.

### 4. Multi-Media Supports

#### 4.1 Audio Data
https://mas.owasp.org/MASTG/demos/android/MASVS-STORAGE/MASTG-DEMO-0005/MASTG-DEMO-0005/#sample

File format and meta information are set with `MediaStore`. Sound recording is triggered with `MediaRecorder`. Sound Play is triggered with `MediaPlayer`.

```java
ContentValues values = new ContentValues(4);
values.put(MediaStore.Audio.Media.DISPLAY_NAME, fileName);
values.put(MediaStore.Audio.Media.DATE_ADDED, (int) (System.currentTimeMillis() / 1000));
values.put(MediaStore.Audio.Media.MIME_TYPE, "audio/mpeg");
values.put(MediaStore.Audio.Media.RELATIVE_PATH, "Recordings");

Uri audiouri = getContentResolver().insert(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, values);
mRecordFile = getContentResolver().openFileDescriptor(audiouri, "w");

// below method is used to initialize
// the media recorder class
mRecorder = new MediaRecorder();
// below method is used to set the audio
// source which we are using a mic.
mRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
// below method is used to set
// the output format of the audio.
mRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
// below method is used to set the
// audio encoder for our recorded audio.
mRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);
// below method is used to set the
// output file location for our recorded audio
mRecorder.setOutputFile(mRecordFile.getFileDescriptor());
try {
    // below method will prepare
    // our audio recorder class
    mRecorder.prepare();
    // start method will start
    // the audio recording.
    mRecorder.start();
} catch (IOException e) {
    Log.e("MediaRecorder Error", "prepare() failed: "+e.getMessage());
}
```

```java
// we are using media player class.
mPlayer = new MediaPlayer();
try {
    // below method is used to set the
    // data source which will be our file name
    mPlayer.setDataSource(mRecordFile.getFileDescriptor());

    // below method will prepare our media player
    mPlayer.prepare();

    // below method will start our media player.
    mPlayer.start();
    statusTV.setText("Recording Started Playing");
} catch (IOException e) {
    Log.e("MediaPlayer Error", "prepare() failed: "+e.getMessage());
}
```

#### 4.2 WAVE format
`AudioRecord` is how you can record a `.wav`, and `.wav` format is not supported in MediaRecorder. Customized `.wav` Android Recorder can be found in this directory: https://github.com/vishwakneelamegam/android-wav.git

### 5. Use File Without Permission
**Remember to check logcat results for `Permission Denied` everytime you first deploy a program. This is the golden rule.**

After Android 10 (API level 29), File Permission shrinks. After Android 14 (API level 34), File Permission requests simply fail.

The latest way to access temperary file for application use, is to call `getExternalFilesDir("Recordings");` inside a class, returning a `File` object. The returned directory and files may not automatically opened. However, the `File` object is what we get full access over.

```java
// prepare a directory
recordDir = getExternalFilesDir("Recordings");
if (!recordDir.exists()) {
    recordDir.mkdirs();
}
// get path
recordPath = recordDir.getPath();
```


### 6. Preformance of Different Build Types
For `Release` Type build (unsigned), the app is 2~3 times faster than `Debug` Type build.

The `Signed` release version is another 50% faster than `Release` (unsigned).

### 7. Layout(xml) and Activity(java)
#### 7.1 Initialization
Each View object in the Activity shall be  found in the xml.
The following example shows how a Button is coordinated.
```xml
<Button
    android:id="@+id/load_button"
    android:layout_width="match_parent"
    android:layout_height="wrap_content"
    android:layout_gravity="center"
    android:layout_marginTop="110dp"
    android:layout_marginBottom="40dp"
    android:background="#3bb5ee"
    android:text="加载模型"
    android:onClick="loadModel">
</Button>
```
The `@+id/load_button` term defines the id of the button in the view.
```java
mLoadButton = findViewById(R.id.load_button);
```

### 8. Layout & View (xml specification)
#### 8.0 Content
1. `ScrollView`: Make a scrollable View that can be scrolled up and down. Only 1 Layout can affiliate to it, whose layout_gravity shall be set `fill`: `android:layout_gravity="center|fill"`.
2. `LinearLayout`: A horizontal one or a vertical one, set by `android:orientation=`.
3. `ListView`: A List object that can display selection and alternatives.
4. `ArrayAdapter`: `ArrayAdapter` is a class in Android that extends `BaseAdapter` and is used to display a list of items in a `ListView` or `Spinner`. It is a convenient way to bind an array of data to a `ListView` or `Spinner` and handle the display of each item.
5. `@string/xxx`: `@string` is a reference to a string resource defined in the `.xml` file. This file is located in the `res/values` directory of your Android project. Referred to in java by `R.string.xxx`.
6. `EditText`: A very useful text input View. `android:hint=` can be used to set the hint message before user input, which can be modified after using `seteHint`.

#### 8.1 Visibility
Visibility State:

***VISIBLE***: The widget is visible and occupies space in the layout.
***INVISIBLE***: The widget is not visible but still occupies space in the layout.
***GONE***: The widget is not visible and does not occupy any space in the layout.

### 9. Unrooted Ways to Get Hardware Info
#### 9.1 CPU temperature
Stored in directory `/sys/class/thermal/thermal_zone*` are temperature info of all thermal zones, each with a `type` field and a `temp` field. `type` is the name of the thermal zone while `temp` is the actual temperature term in milli-Celsius/Celsius. Below is the code of 1 way to read it.

You can have a double check for the type name with `cat` command. 
```bash
adb shell "cat /sys/class/thermal/thermal_zone*/temp"
```

As for Huawei Mate40 Pro, the proper thermal zone can be `cluster2`, while for Meizu 21, the proper thermal zone is `cpu-2-0-1`.
```java
private float getCPUTemperature() {
    float tempInCelsius = 0;
    try {
        for (int i=0; i<30; ++i) {
            BufferedReader reader = new BufferedReader(new FileReader(String.format("/sys/class/thermal/thermal_zone%d/type", i)));
            String type = reader.readLine();
            reader = new BufferedReader(new FileReader(String.format("/sys/class/thermal/thermal_zone%d/temp", i)));
            String temperature = reader.readLine(); // Read the first line of the file
            // Convert the temperature from millidegrees Celsius to degrees Celsius if necessary.
            tempInCelsius = (float) Integer.parseInt(temperature);
            if (tempInCelsius>1000)  {
                tempInCelsius = tempInCelsius/ 1000.0f;
            }
            if (type.contains("cpu-") || type.contains("cluster") || type.contains("cpu0")) {
                break;
            }
    } catch (IOException e) {
        Log.e("Error", "Bad Read");
    }
    Log.i("Temperature", "CPU Temperature: " + tempInCelsius + "°C");
    return tempInCelsius;
}
```

* Note that HardwarePropertiesManager mentioned in the following url is inaccessible without root access.
https://stackoverflow.com/questions/51252807/android-api-to-check-if-the-cpu-is-slowed-down
https://developer.android.com/reference/android/os/HardwarePropertiesManager#DEVICE_TEMPERATURE_CPU

#### 9.2 Battery Charge Level & Voltage
https://developer.android.com/reference/android/os/BatteryManager

The current battery charge level and voltage is broadcast upon changes, if a BroadcastReceiver is set up as below (IntentFilter set to filter ACTION_BATTERY_CHANGED):

```java
IntentFilter intentfilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
Intent batteryStatus = this.registerReceiver(broadcastreceiver, intentfilter);

private BroadcastReceiver broadcastreceiver = new BroadcastReceiver() {
    @Override
    public void onReceive(Context context, Intent intent) {
        mBatteryVoltage = (float) (intent.getIntExtra(BatteryManager.EXTRA_VOLTAGE,0)) / 1000f; // mV -> V
        mChargeNow = (int) (intent.getIntExtra(BatteryManager.EXTRA_LEVEL,0)); // charge level
    }
};
```

#### 9.3 Battery Current Input Current (positive for net input)

The current input current (positive for net input) can be only monitor at real-time inside another opening thread (a timer task) for every time interval. A good way to monitor it is to define a class, extending `TimerTask`.

```java
public class EnergyMonitor extends TimerTask {
    EnergyMonitor(Context parent) {
        uAList = new ArrayList<Integer>();
        VList = new ArrayList<Float>();
        ctx = parent;
    }

    @Override
    public void run() {
        int mBatteryCurrent = ((BatteryManager) ctx.getSystemService(Context.BATTERY_SERVICE)).getIntProperty(BatteryManager.BATTERY_PROPERTY_CURRENT_NOW);
        if (Math.abs(mBatteryCurrent) <= 10000) { mBatteryCurrent *= 1000; } // convert mA -> uA. (device heterogeneity resolution.)
        uAList.add(mBatteryCurrent);
    }
}
```

Note that only the `MainActivity` can make calls to `getSystemService` and access BatteryManager, so that itself shall be passed to the timer task during initialization.

Such initialization is inside `MainActivity`.

```java
public class MainActivity extends AppCompatActivity {
    private void startEnergyTracing() {
        energyTimer = new Timer();
        energyMonitor = new EnergyMonitor(this);
        energyMonitor.resetInfo();
        energyTimer.scheduleAtFixedRate(energyMonitor, 0, energySampleInterval);
    }

    private void endEnergyTracing() {
        try {
            energyTimer.cancel();
        } catch (Exception e) {
            Log.e("endEnergyTracing", e.getMessage());
        }
    }
}
```

### 10. Wrap data files into app
* Access file by Resource ID: put it under `src/main/res/raw`.
* Access file by original path/name: put it under `src/main/assets`.

### 11. Wrap pre-built libraries into app
1. Write `copy` commands in your `CMakeLists.txt` or `build.gradle`, so that the libraries are available under `src/main/jni/libs/arm64-v8a` or any jniLib path specified.
This path can be referred to as `getApplicationInfo().nativeLibraryDir`.
2. All the libraries under the path can be automatically linked by linke. However, if your lib is to be accessed by `dlopen`, it can't be punched. Add 
```gradle
    packagingOptions {
        jniLibs.useLegacyPackaging = true
    }
```
after `externalNativeBuild`, so that it won't be compressed.
3. System libraries
In `AndroidManifest.xml`, you may specify to link system native libraries such as OpenCL. If `android:required="true"`, the app crashed if the lib is not available.
```xml
<uses-native-library
   android:name="libOpenCL.so"
   android:required="true" />
<uses-native-library android:name="libOpenCL-car.so" android:required="false"/>
<uses-native-library android:name="libOpenCL-pixel.so" android:required="false"/>
<uses-library
   android:name="libcdsprpc.so"
   android:required="false"/>
```
4. Conflicts
System libraries may conflict with your own libraries if they are the same name, such as `libQnnHtpV79Skel.so`. To avoid the conflicts, specifically Qualcomm SDK looks into `${ADSP_LIBRARY_PATH}` for these libraries runtime. Set the environment variables can resolve this.

```java
Os.setenv("ADSP_LIBRARY_PATH", getApplicationInfo().nativeLibraryDir, true);
```

### 12. redirect std cout and cerr to logcat
Your code may not target Android initially, std cout and cerr are no where to be found if you don't redirect them.
```cpp
class LogcatBuffer : public std::streambuf {
    std::string buffer;

protected:
    int overflow(int c) override {
        if (c == '\n') {
            __android_log_print(ANDROID_LOG_INFO, "CPP", "%s", buffer.c_str());
            buffer.clear();
        } else if (c != EOF) {
            buffer.push_back(c);
        }
        return c;
    }

    int sync() override {
        if (!buffer.empty()) {
            __android_log_print(ANDROID_LOG_INFO, "CPP", "%s", buffer.c_str());
            buffer.clear();
        }
        return 0;
    }
};

static LogcatBuffer logcat_buffer;

void redirectStdToLogcat() {
    std::cout.rdbuf(&logcat_buffer);
    std::cerr.rdbuf(&logcat_buffer);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    redirectStdToLogcat();
    __android_log_print(ANDROID_LOG_INFO, "MNN_DEBUG", "JNI_OnLoad");
    return JNI_VERSION_1_4;
}
```

The overrode `overflow` and `sync` functions helps the buffer to know the `flush` behaviour (i.e., to `__android_log_print`).