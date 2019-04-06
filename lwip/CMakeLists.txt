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
    ${LWIP_PATH}/src/core/def.c
    ${LWIP_PATH}/src/core/dns.c
    ${LWIP_PATH}/src/core/inet_chksum.c
    ${LWIP_PATH}/src/core/init.c
    ${LWIP_PATH}/src/core/mem.c
    ${LWIP_PATH}/src/core/memp.c
    ${LWIP_PATH}/src/core/netif.c
    ${LWIP_PATH}/src/core/pbuf.c
    ${LWIP_PATH}/src/core/stats.c
    ${LWIP_PATH}/src/core/sys.c
    ${LWIP_PATH}/src/core/tcp.c
    ${LWIP_PATH}/src/core/tcp_in.c
    ${LWIP_PATH}/src/core/tcp_out.c
    ${LWIP_PATH}/src/core/timeouts.c
    ${LWIP_PATH}/src/core/udp.c
    ${LWIP_PATH}/src/core/ip.c
    ${LWIP_PATH}/src/core/ipv4/icmp.c
    ${LWIP_PATH}/src/core/ipv4/ip4.c
    ${LWIP_PATH}/src/core/ipv4/ip4_addr.c
    ${LWIP_PATH}/src/core/ipv4/ip4_frag.c
#    ${LWIP_PATH}/src/core/ipv6/ip6.c
#    ${LWIP_PATH}/src/core/ipv6/nd6.c
#    ${LWIP_PATH}/src/core/ipv6/icmp6.c
#    ${LWIP_PATH}/src/core/ipv6/ip6_addr.c
#    ${LWIP_PATH}/src/core/ipv6/ip6_frag.c
    ${LWIP_PATH}/src/api/api_lib.c
    ${LWIP_PATH}/src/api/api_msg.c
    ${LWIP_PATH}/src/api/err.c
    ${LWIP_PATH}/src/api/netdb.c
    ${LWIP_PATH}/src/api/tcpip.c
)

set(CONTRIB_SOURCES
    ${CONTRIB_PATH}/ports/unix/port/perf.c
    ${CONTRIB_PATH}/ports/unix/port/sys_arch.c
    ${CONTRIB_PATH}/ports/unix/port/netif/list.c
)
add_library (lwip SHARED
        ${LWIP_SOURCES}
        ${CONTRIB_SOURCES} )

