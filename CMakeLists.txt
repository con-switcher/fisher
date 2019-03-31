cmake_minimum_required(VERSION 3.0)
project(lwip-bin)

include_directories(
        out/lwip/include
)
link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/out/lwip/
)

# lwip文件
set(LWIP_SOURCES
        src/lwip.c
)

add_executable(lwip-bin ${LWIP_SOURCES})
target_link_libraries(lwip-bin lwip pthread)


