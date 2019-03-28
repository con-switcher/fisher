cmake_minimum_required (VERSION 3.13)
project (lwip)

include_directories(
    out/lwip/include
)

# lwip文件
set(LWIP_SOURCES
    src/lwip.cpp
)

add_library (lwip ${LWIP_SOURCES} )
