cmake_minimum_required(VERSION 3.13)
project(lwip)

include_directories(
        out/lwip/include
)
link_directories(
        out/lwip/
)

# lwip文件
set(LWIP_SOURCES
        src/lwip.cpp
        )

add_executable(lwip ${LWIP_SOURCES})
target_link_libraries(lwip liblwip.a)
