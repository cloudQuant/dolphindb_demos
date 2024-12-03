#include "DolphinDB.h"
#include "Util.h"
#include <iostream>
using namespace dolphindb;

int main(int argc, char* argv[]) {
    DBConnection conn;
    bool ret = conn.connect("127.0.0.1", 8848, "admin", "123456");
    if (!ret) {
        std::cout << "Failed to connect to the server" << std::endl;
        return 0;
    }
    ConstantSP s = Util::createString("123");
    conn.upload("s", s);
    ConstantSP r = conn.run("s");
    std::cout << r->getString() << std::endl;
    return 0;
}