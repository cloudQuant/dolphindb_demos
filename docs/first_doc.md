1. linux编译so文件指令
```bash
# 在主目录下运行
mkdir build && cd build && 
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_AERON=OFF -DUSE_OPENSSL=ON && 
make -j4 &&
cp libDolphinDBAPI.so ../lib
```
