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
#endif  // INCLUDE_CCAPI_CPP_CCAPI_UTIL_H_
