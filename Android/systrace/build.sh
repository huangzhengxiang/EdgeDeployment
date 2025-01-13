if !(test -d build) then
    mkdir build
fi
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
 -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="arm64-v8a" \
  -DANDROID_STL=c++_static -DCMAKE_CXX_STANDARD=17 \
   -DANDROID_NATIVE_API_LEVEL=android-28 -DNATIVE_LIBRARY_OUTPUT=.
make
cp ./trace ../release/