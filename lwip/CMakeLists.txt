cmake_minimum_required(VERSION 3.0)
project (lwip)

set(LWIP_PATH "lwip-2.1.2")
set(CONTRIB_PATH "contrib-2.1.0")



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

