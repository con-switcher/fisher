
#include <iostream>

#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/err.h"
#include "lwip/init.h"
#include "lwip/ip_addr.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/sys.h"

int main(int argc, char **argv)
{
    ip_addr_t ip, netmask, gw;
    netif interface;

    memset(&interface, 0, sizeof(netif));

    lwip_init();

    std::cout<< "hihi" << std::endl;

    return (0);
}