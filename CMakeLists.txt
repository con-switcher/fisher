cmake_minimum_required(VERSION 3.0)
project(lwip-bin)
set (CMAKE_CXX_STANDARD 11)
include_directories(
        out/lwip/include
        out/libuv/include
)


# lwip文件
set(LWIP_SOURCES
        src/main.cpp
)

add_executable(lwip-bin ${LWIP_SOURCES})


