#ifndef INCLUDE_CCAPI_CPP_CCAPI_UTIL_H_
#define INCLUDE_CCAPI_CPP_CCAPI_UTIL_H_
#include <chrono>
#include <sstream>
namespace ccapi {
/*
 * TimePoint：定义一个类型别名 TimePoint，表示时间点类型, 使用系统时钟 (std::chrono::system_clock) 和纳秒精度 (std::chrono::nanoseconds)
 */
typedef std::chrono::time_point<std::chrono::system_clock,
                                std::chrono::nanoseconds> TimePoint;
} /* namespace ccapi */
// 为 TimePoint 添加 cout 输出功能
std::ostream& operator<<(std::ostream& os, const ccapi::TimePoint& tp) {
    // 将 TimePoint 转换为 std::chrono::system_clock 类型
    auto systemTimePoint = std::chrono::time_point_cast<std::chrono::seconds>(tp);
    std::time_t t = std::chrono::system_clock::to_time_t(systemTimePoint);

    os << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");

    // 添加毫秒信息
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() % 1000;
    os << "." << std::setw(3) << std::setfill('0') << millis;

    // 添加微秒信息
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()).count() % 1000;
    os << "." << std::setw(3) << std::setfill('0') << micros;

    // 添加纳秒信息
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count() % 1000;
    os << "." << std::setw(3) << std::setfill('0') << nanos;

    return os;
}
#endif  // INCLUDE_CCAPI_CPP_CCAPI_UTIL_H_
