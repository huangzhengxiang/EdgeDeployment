## Android

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

### 3. LogCat
#### C/C++
由于在Android中不能直接使用cout来输出日志，需要使用Android的日志系统。在JNI代码中包含<android/log.h>头文件，并使用__android_log_print函数来输出日志

#### Java
import android.util.Log;
Log.e([Tag], [msg])
Log.d([Tag], [msg])
Log.i([Tag], [msg])

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

### 6. Coordinate Layout(xml) and Activity(java)
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
