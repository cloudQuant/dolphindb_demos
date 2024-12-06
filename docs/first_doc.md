1. some packages needed to install
   ```bash
   # openssl
   sudo apt-get install libssl-dev
   # uuid
   sudo apt-get install uuid-dev
   # zlib
   sudo apt-get install zlib1g-dev
   ```
2. linux编译so文件指令
    ```bash
    # 在主目录下运行
    mkdir build && cd build && 
    cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_AERON=OFF -DUSE_OPENSSL=ON && 
    make -j4 &&
    cp libDolphinDBAPI.so ../examples/lib
    ```
2. linux下编译单个文件
    ```bash
    cmake -B build -DABI=0 -DUSE_OPENSSL=1 &&
    cmake --build build
    ```