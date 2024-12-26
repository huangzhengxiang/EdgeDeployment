## OpenCL

### 1. add into android studio project
https://github.com/alibaba/MNN/issues/2023
https://developer.android.google.cn/guide/topics/manifest/uses-native-library-element?hl=zh-cn
Need to add to `xml` `<uses_native_libarary>` after API level 31.

Example
```xml
    <!-- inside application -->
    <uses-native-library
        android:name="libOpenCL.so"
        android:required="false" />
```