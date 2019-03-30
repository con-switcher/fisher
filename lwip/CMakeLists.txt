cmake_minimum_required(VERSION 3.0)
project (lwip)

set(LWIP_DIR "lwip")
set(CONTRIB_DIR "contrib")



include_directories(
    ${LWIP_DIR}/src/include
    ${LWIP_DIR}/src/include/ipv4
    ${LWIP_DIR}/src/include/ipv6
    ${CONTRIB_DIR}/ports/unix/include
    custom
)

set(lwipcore_SRCS
        ${LWIP_DIR}/src/core/def.c
        ${LWIP_DIR}/src/core/dns.c
        ${LWIP_DIR}/src/core/inet_chksum.c
        ${LWIP_DIR}/src/core/init.c
        ${LWIP_DIR}/src/core/mem.c
        ${LWIP_DIR}/src/core/memp.c
        ${LWIP_DIR}/src/core/netif.c
        ${LWIP_DIR}/src/core/pbuf.c
        ${LWIP_DIR}/src/core/stats.c
        ${LWIP_DIR}/src/core/sys.c
        ${LWIP_DIR}/src/core/tcp.c
        ${LWIP_DIR}/src/core/tcp_in.c
        ${LWIP_DIR}/src/core/tcp_out.c
        ${LWIP_DIR}/src/core/timers.c
        ${LWIP_DIR}/src/core/udp.c
)
set(lwipcore4_SRCS
        ${LWIP_DIR}/src/core/ipv4/icmp.c
        ${LWIP_DIR}/src/core/ipv4/ip4.c
        ${LWIP_DIR}/src/core/ipv4/ip4_addr.c
        ${LWIP_DIR}/src/core/ipv4/ip_frag.c
)
set(lwipapi_SRCS
        ${LWIP_DIR}/src/api/api_lib.c
        ${LWIP_DIR}/src/api/api_msg.c
        ${LWIP_DIR}/src/api/err.c
        ${LWIP_DIR}/src/api/netdb.c
        ${LWIP_DIR}/src/api/tcpip.c
)

set(CONTRIB_SOURCES
        ${CONTRIB_DIR}/ports/unix/lwip_chksum.c
        ${CONTRIB_DIR}/ports/unix/perf.c
        ${CONTRIB_DIR}/ports/unix/sys_arch.c
        ${CONTRIB_DIR}/ports/unix/netif/list.c
        ${CONTRIB_DIR}/ports/unix/netif/tcpdump.c
)
add_library (lwip STATIC
        ${lwipcore_SRCS} ${lwipcore4_SRCS} ${lwipapi_SRCS}
        ${CONTRIB_SOURCES} )
