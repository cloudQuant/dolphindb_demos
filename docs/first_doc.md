1. linux编译so文件指令
```bash
mkdir build && cd build && 
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_AERON=OFF -DUSE_OPENSSL=ON && 
make -j4
```