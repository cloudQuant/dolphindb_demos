cmake_minimum_required(VERSION 3.13)

set(ProjectName binance_public_subscription_test1)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LINK_LIBS)
if(USE_OPENSSL)
    add_definitions("-DUSE_OPENSSL=1")
    find_package(OpenSSL REQUIRED)
    include_directories(${OPENSSL_INCLUDE_DIR})
    message("openssl include dir    : ${OPENSSL_INCLUDE_DIR}")
    message("openssl ssl lib dir    : ${OPENSSL_SSL_LIBRARY}")
    message("openssl crypto lib dir : ${OPENSSL_CRYPTO_LIBRARY}")
endif()
if(UNIX)
    find_path(UUID_INCLUDE_DIR uuid/uuid.h)
    if (NOT UUID_INCLUDE_DIR)
        message(FATAL_ERROR "Failed to find uuid/uuid.h")
    endif()
    add_compile_options(-std=c++2a -DLINUX -Wall -fPIC)
    list(APPEND LINK_LIBS DolphinDBAPI rt pthread)
elseif(WIN32)
    if(MSVC)
        add_compile_options(-DWINDOWS -DNOMINMAX)
        list(APPEND LINK_LIBS DolphinDBAPI)
    elseif(MINGW)
        add_compile_options(-std=c++20 -DWINDOWS -Wall -fPIC)
        list(APPEND LINK_LIBS DolphinDBAPI pthread)
    endif()
endif()

add_compile_definitions(CCAPI_ENABLE_SERVICE_MARKET_DATA)
add_compile_definitions(CCAPI_ENABLE_EXCHANGE_BINANCE_USDS_FUTURES)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/)
add_executable(${ProjectName} ${PROJECT_SOURCE_DIR}/main.cpp)
add_dependencies(${ProjectName} boost rapidjson)
# target_compile_features(${ProjectName} PRIVATE cxx_std_11)
target_link_libraries(${ProjectName} ${LINK_LIBS})

# Set the output executable name to the project name
set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME ${ProjectName})

# Set the output path for the executable
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # C4251: Class member of an exported class is not marked dllexport.
    # This is triggered by std::string but we cannot modify STL.
    # C4100: Parameter is unused. We should fix this later.
    # C4996: The code uses a deprecated API. We should fix this later.
    target_compile_options(${ProjectName} PRIVATE /W3 /MP /wd4251 /wd4100 /wd4996)
    # Unicode support of fmt (used in spdlog) requires /utf-8 of MSVC.
    target_compile_definitions(${ProjectName} PRIVATE FMT_UNICODE=0)
else()
    # overloaded-virtual: Derived class hides base class virtual functions
    # This is triggered by derived classes of Constant and we will fix this later
    target_compile_options(${ProjectName} PRIVATE -Wall -Wno-overloaded-virtual)
    # Enable Wextra and Wconversion when possible
    # Try clang hwasan when possible: -fsanitize=hwaddress -Wl,--no-relax
    set(CMAKE_CXX_FLAGS_DEV "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -Werror -Wpedantic")
endif()