cmake_minimum_required(VERSION 3.0)
project (lwip)

set(LWIP_PATH "lwip-2.1.2")
set(CONTRIB_PATH "contrib-2.1.0")

list(APPEND uv_cflags -fvisibility=hidden --std=gnu89)
list(APPEND uv_cflags -Wall -Wextra -Wstrict-prototypes)
list(APPEND uv_cflags -Wno-unused-parameter)

include_directories(
    ${LWIP_PATH}/src/include 
    ${CONTRIB_PATH}/ports/unix/port/include
    custom
)

# lwip文件
set(LWIP_SOURCES

    ${LWIP_PATH}/src/core/ip.c

)

set(CONTRIB_SOURCES
    ${CONTRIB_PATH}/ports/unix/port/perf.c
    ${CONTRIB_PATH}/ports/unix/port/sys_arch.c
    ${CONTRIB_PATH}/ports/unix/port/netif/list.c
)
add_library (lwip STATIC
        ${LWIP_SOURCES}
       )

target_compile_options(lwip PRIVATE ${uv_cflags})
