cmake_minimum_required(VERSION 3.16)

set(ProjectName apiDemo)
project(${ProjectName})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_LIST
        # ${CMAKE_CURRENT_SOURCE_DIR}/examples/01_connect_to_dolphindb/main.cpp
        main.cpp
)

set(LINK_LIBS)
set(CMAKE_BUILD_RPATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
if(USE_OPENSSL)
    add_definitions("-DUSE_OPENSSL=1")
    if(OPENSSL_PATH)
        set(ENV{OPENSSL_ROOT_DIR} ${OPENSSL_PATH})
    endif()
    find_package(OpenSSL REQUIRED)
    include_directories(${OPENSSL_INCLUDE_DIR})
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

# 输出指定文件夹中的文件名
file(GLOB_RECURSE LIB_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../lib/*")
message(STATUS "Files in ${CMAKE_CURRENT_SOURCE_DIR}/../lib:")
foreach(FILE ${LIB_FILES})
    message(STATUS "  ${FILE}")
endforeach()

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../lib
)

add_executable(${ProjectName} ${SRC_LIST})

# 确保链接时包含 libDolphinDBAPI.so
target_link_libraries(${ProjectName}
        ${LINK_LIBS}
        ${OPENSSL_LIBRARIES}
        DolphinDBAPI
        # ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libDolphinDBAPI.so
)

# 设置可执行文件的输出路径
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
