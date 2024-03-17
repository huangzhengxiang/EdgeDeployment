## MNN Hacking
~~~bash
git clone https://github.com/alibaba/MNN.git
~~~

### 1. Build for memory monitoring
Debug Memory is Enabled.
~~~bash
mkdir debug
cd debug
cmake .. -DCMAKE_CXX_STANDARD=17 -DMNN_USE_SYSTEM_LIB=OFF -DMNN_BUILD_SHARED_LIBS=ON -DMNN_BUILD_TRAIN=ON -DMNN_BUILD_QUANTOOLS=ON -DMNN_EVALUATION=ON -DMNN_BUILD_CONVERTER=ON -DMNN_DEBUG_MEMORY=ON -DMNN_DEBUG_TENSOR_SIZE=ON -DMNN_PORTABLE_BUILD=ON -DTFMODEL_OPTIMIZE=ON -DMNN_BUILD_LLM=ON -DMNN_SUPPORT_TRANSFORMER_FUSE=ON -DMNN_LOW_MEMORY=ON -DMNN_AVX512=ON
make -j20
~~~
You shall export asan ld path before executing.
~~~bash
export LD_PRELOAD=/path/to/libasan.so
~~~ 

### 2. Build for ready to use release version
Release version, minimal verbose, used to be the underlying library.
~~~bash
mkdir release
cd release
cmake .. -DCMAKE_CXX_STANDARD=17 -DMNN_USE_SYSTEM_LIB=OFF -DMNN_BUILD_SHARED_LIBS=ON -DMNN_BUILD_TRAIN=ON -DMNN_BUILD_QUANTOOLS=ON -DMNN_EVALUATION=ON -DMNN_BUILD_CONVERTER=ON -DMNN_DEBUG_MEMORY=OFF -DMNN_DEBUG_TENSOR_SIZE=OFF -DMNN_PORTABLE_BUILD=ON -DTFMODEL_OPTIMIZE=ON -DMNN_BUILD_LLM=ON -DMNN_SUPPORT_TRANSFORMER_FUSE=ON -DMNN_LOW_MEMORY=ON -DMNN_AVX512=ON
make -j20
~~~


## MNN-LLM Hacking
Git clone mnn-llm.
~~~bash
git clone https://github.com/wangzhaode/mnn-llm.git
git pull origin kv_chunk:kv_chunk
git checkout kv_chunk
~~~

Soft link .so and .h from MNN.
~~~bash
ln -s /path/to/MNN/include/MNN include
ln -s /path/to/MNN/build/libMNN.so /path/to/MNN/build/express/libMNN_Express.so libs
~~~

Then, build.
~~~bash
mkdir build
cd build
cmake ..
make -j20
cd ..
~~~

Download models.
~~~bash
./download.sh qwen-7b-chat
~~~

Run.
~~~bash
./cli_demo ../resource/models/qwen-7b-chat ../resource/prompt.txt
~~~