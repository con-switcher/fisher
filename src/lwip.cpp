
#include <iostream>
#include <cstring>

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
    struct netif netif;

    memset(&netif, 0, sizeof(netif));

    lwip_init();

    ip_addr_set_zero(&netmask);
    ip_addr_set_zero(&gw);
    netif_add(&netif, &ip, &netmask, &gw, s, init_oc_netif, ip_input);
    netif.mtu = ocp_atoi(mtu_str);

    netif_set_default(&netif);
    netif_set_up(&netif);

    std::cout<< "hihi" << std::endl;

    return (0);
}