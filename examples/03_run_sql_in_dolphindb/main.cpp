#include "DolphinDB.h"
#include "Util.h"
#include <iostream>
using namespace dolphindb;
int main(int argc, const char **argv)
{
    DBConnection conn;
    bool ret = conn.connect("127.0.0.1", 8848, "admin", "123456");
    if (!ret) {
        std::cout << "Failed to connect to the server" << std::endl;
        return 0;
    }
    conn.run(R"(
        n=1000000
        ID=rand(10, n)
        x=rand(1.0, n)
        t=table(ID, x)
        db=database(directory="dfs://hashdb", partitionType=HASH, partitionScheme=[INT, 2])
        pt = db.createPartitionedTable(t, `pt, `ID)
        pt.append!(t);
    )");
    ConstantSP re = conn.run("select count(x) from pt;");
    std::cout << "the rows of table = " << re->getString() << std::endl;
    return 0;
}