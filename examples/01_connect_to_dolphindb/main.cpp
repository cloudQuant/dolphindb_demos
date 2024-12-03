#include "DolphinDB.h"
#include <iostream>
using namespace dolphindb;

int main(int argc, char* argv[]) {
    DBConnection conn;
    bool ret = conn.connect("127.0.0.1", 8848, "admin", "8848");
    if (!ret) {
        std::cout << "Failed to connect to the server" << std::endl;
        return 0;
    }
    ConstantSP result= conn.run("1+1");
    std::cout << result->getString() << std::endl;
    return 0;
}