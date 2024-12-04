#include "DolphinDB.h"
#include "MultithreadedTableWriter.h"
#include "Util.h"
#include "Types.h"
#include "ScalarImp.h" // 基本数据类型
#include "Set.h"
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>  // 添加这个头文件
using namespace dolphindb;
using namespace std;
using namespace std::chrono;
using std::this_thread::sleep_for;




int main(){
    DBConnection conn;
    conn.connect("127.0.0.1", 8848, "admin", "123456");
    /*待插入的数据：
    2024.09.17 09:43:32	1,726,537,412,001	1,726,537,409.7140648	ACHUSDT	0.017791	0.01780094	0.01778485
    */
    //构造MTW对象
    ErrorCodeInfo errorInfo;
    MultithreadedTableWriter writer("127.0.0.1", 8848, "admin", "123456", "dfs://binance_data", "mark_price", false);
    std::string datetimeStr = "2024.09.17 09:43:32";
    // 将字符串转换为 tm 结构
    std::tm tm = {};
    std::istringstream ss(datetimeStr);
    ss >> std::get_time(&tm, "%Y.%m.%d %H:%M:%S");

    if (ss.fail()) {
        std::cerr << "Failed to parse date string" << std::endl;
        return -1;
    }

    // 将 tm 结构转换为 time_t
    std::time_t time = std::mktime(&tm);

    // 将 time_t 转换为 DolphinDB 的 DATETIME 类型
    // DolphinDB 的 DATETIME 类型通常是自 1970 年 1 月 1 日以来的毫秒数
    long long dolphinDBDateTime = static_cast<long long>(time) * 1000;

    // 创建 DolphinDB 的 ConstantSP 对象
    ConstantSP datetime = new Long(dolphinDBDateTime);
    ConstantSP server_time = new Long(1726537412001);
    ConstantSP local_update_time = new Double(1726537409.7140648);
    ConstantSP symbol = new String("ACHUSDT");
    ConstantSP mark_price = new Double(0.017791);
    ConstantSP index_price = new Double(0.01780094);
    ConstantSP settlement_price = new Double(0.01778485);

    if(!writer.insert(errorInfo, datetime, server_time, local_update_time, symbol, mark_price, index_price, settlement_price)){
        std::cout << "insert fail " << errorInfo.errorInfo << std::endl;
        return -1;
    }
    writer.waitForThreadCompletion();
    //插入完成
    MultithreadedTableWriter::Status status;
    writer.getStatus(status);
    if (status.hasError()) {
        std::cout << "error in writing: " << status.errorInfo << std::endl;
    }
    // 验证插入结果
    std::cout << "run success" << std::endl;
    return 0;
}