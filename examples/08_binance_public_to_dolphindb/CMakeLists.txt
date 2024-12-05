cmake_minimum_required(VERSION 3.16)
set(NAME binance_public)
project(${NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT APPLE AND NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if (WIN32)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
get_filename_component(CCAPI_PROJECT_DIR ../ ABSOLUTE)
message(STATUS "CCAPI_PROJECT_DIR: ${CCAPI_PROJECT_DIR}")

set(SRC_LIST
        main.cpp
)

set(LINK_LIBS)
if(USE_OPENSSL)
    add_definitions("-DUSE_OPENSSL=1")
    if(OPENSSL_PATH)
        set(ENV{OPENSSL_ROOT_DIR} ${OPENSSL_PATH})
    endif()
    find_package(OpenSSL REQUIRED)
    include_directories(${OPENSSL_INCLUDE_DIR})
    list(APPEND LINK_LIBS ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
    message("openssl include dir    : ${OPENSSL_INCLUDE_DIR}")
    message("openssl ssl lib dir    : ${OPENSSL_SSL_LIBRARY}")
    message("openssl crypto lib dir : ${OPENSSL_CRYPTO_LIBRARY}")
endif()

if(UNIX)
    add_compile_options(-std=c++2a -DLINUX -Wall -O0 -g -fPIC -Wl,-rpath,${CMAKE_CURRENT_SOURCE_DIR}:${CMAKE_CURRENT_SOURCE_DIR}/lib)
    if(ABI EQUAL 0)
        message("set _GLIBCXX_USE_CXX11_ABI to 0")
        add_definitions("-D_GLIBCXX_USE_CXX11_ABI=0")
    elseif(ABI EQUAL 1)
        message("set _GLIBCXX_USE_CXX11_ABI to 1")
        add_definitions("-D_GLIBCXX_USE_CXX11_ABI=1")
    endif()
    list(APPEND LINK_LIBS DolphinDBAPI rt pthread)
elseif(WIN32)
    if(MSVC)
        add_compile_options(-DWINDOWS -DNOMINMAX -D_DEBUG)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MDd")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
        list(APPEND LINK_LIBS DolphinDBAPI)
    elseif(MINGW)
        add_compile_options(-std=c++20 -DWINDOWS -Wall -O0 -g -fPIC -Wl,-rpath,${CMAKE_CURRENT_SOURCE_DIR}:${CMAKE_CURRENT_SOURCE_DIR}/lib)
        if(ABI EQUAL 0)
            message("set _GLIBCXX_USE_CXX11_ABI to 0")
            add_definitions("-D_GLIBCXX_USE_CXX11_ABI=0")
        elseif(ABI EQUAL 1)
            message("set _GLIBCXX_USE_CXX11_ABI to 1")
            add_definitions("-D_GLIBCXX_USE_CXX11_ABI=1")
        endif()
        list(APPEND LINK_LIBS DolphinDBAPI pthread)
    endif()
endif()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../lib/
)

add_compile_definitions(CCAPI_ENABLE_SERVICE_MARKET_DATA)
add_compile_definitions(CCAPI_ENABLE_EXCHANGE_BINANCE_USDS_FUTURES)

add_executable(${NAME} ${SRC_LIST})

target_link_libraries(${NAME} ${LINK_LIBS})

