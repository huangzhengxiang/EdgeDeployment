## iOS native

### 1. framework build

#### 1.1 overview
basic example routine:
```cmake
add_library(mnn_wrapper ${SRCS} ${HDRS})
set_target_properties(mnn_wrapper PROPERTIES FRAMEWORK TRUE)
set_target_properties(mnn_wrapper PROPERTIES
    MACOSX_FRAMEWORK_IDENTIFIER com.hzx.llm.mnn_wrapper
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PACKAGE_VERSION}
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${PACKAGE_VERSION}
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)
set_target_properties(MNN PROPERTIES MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/../MNN/project/ios/MNN/Info.plist)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_LIST_DIR}/include/llm_wrapper.h PROPERTIES MACOSX_PACKAGE_LOCATION Headers/)
```


#### 1.2 add_lbrary
If your headers are needed in the framework, add it in the `add_library` term, so that it's recognized as part of the framework.

#### 1.3 set_target_properties
First, the library shall be set to be a FRAMEWORK.
```cmake
set_target_properties(mnn_wrapper PROPERTIES FRAMEWORK TRUE)
```

Second, identifier, version, and signature shall be set:
```cmake
set_target_properties(mnn_wrapper PROPERTIES
    MACOSX_FRAMEWORK_IDENTIFIER com.hzx.llm.mnn_wrapper
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PACKAGE_VERSION}
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${PACKAGE_VERSION}
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)
```

***Caution:*** the `MACOSX_FRAMEWORK_IDENTIFIER` field shall be different and unique for each framework.

Third, the `Info.plist` file is also necessary:

```cmake
set_target_properties(MNN PROPERTIES MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/../MNN/project/ios/MNN/Info.plist)
```

#### 1.4 headers
Finally, set headers' paths inside the framework:
```cmake
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_LIST_DIR}/include/llm_wrapper.h PROPERTIES MACOSX_PACKAGE_LOCATION Headers/)
```

They will be referred to by including `mnn_wrapper/llm_wrapper.h` in objc files.

#### 1.5 set toolchain and target version
Get toolchain from https://github.com/cristeab/ios-cmake.git, which is a fork of https://code.google.com/p/ios-cmake/. Which in turn is based off of the Platform/Darwin.cmake and Platform/UnixPaths.cmake files which are included with CMake 2.8.4 (just as what MNN does)

Specify the development target by `-DDEPLOYMENT_TARGET=16.4` when cmake.