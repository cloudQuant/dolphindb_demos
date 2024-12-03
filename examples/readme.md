### how to get include and lib documents
参考: https://docs.dolphindb.cn/zh/cppdoc/compiling_dll_so.html

### how to compile dolphindb_cpp_api
如编译第一个例子，使用如下命令
1. cd dolphindb_cpp_api/01_connect_to_dolphindb
2. mkdir build && cd build && cmake .. -DABI=0 -DUSE_OPENSSL=1 && cmake --build . && cd ..
3. 编译过后的文件就在bin文件夹下