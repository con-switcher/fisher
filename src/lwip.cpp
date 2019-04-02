
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
#include "lwip/ip.h"

int main(int argc, char **argv)
{
    ip_addr_t ip, netmask, gw;
    struct netif netif;

    memset(&netif, 0, sizeof(netif));

    lwip_init();

    ip_addr_set_zero(&netmask);
    ip_addr_set_zero(&gw);
    ipaddr_aton("127.0.0.1", &ip);
    netif_add(&netif, &ip, &netmask, &gw, s, init_netif, ip_input);
    netif.mtu = ocp_atoi(mtu_str);

    netif_set_default(&netif);
    netif_set_up(&netif);

    std::cout<< "hihi" << std::endl;

    return (0);
}

static err_t init_netif(struct netif *netif)
{
    netif->name[0] = 'u';
    netif->name[1] = 'n';
    netif->output = lwip_data_out;
    return ERR_OK;
}

static err_t lwip_data_out(struct netif *netif, struct pbuf *p, ip_addr_t *ipaddr)
{
    struct ocp_sock *s = netif->state;
    int i = 0, total = 0;
    ssize_t ret;
    struct iovec iov[MAX_IOVEC];

    if (tcpdump_enabled)
        tcpdump(p);

    for (; p; p = p->next) {
        if (i >= MAX_IOVEC) {
            warn("%s: too many chunks, dropping packet\n", __func__);
            return ERR_OK;
        }
        iov[i].iov_base = p->payload;
        iov[i++].iov_len = p->len;
        total += p->len;
    }

    ret = writev(s->fd, iov, i);
    if (ret < 0) {
        if (errno == ECONNREFUSED || errno == ENOTCONN)
            vpn_conn_down();
        else
            LINK_STATS_INC(link.drop);
    } else if (ret != total)
        LINK_STATS_INC(link.lenerr);
    else
        LINK_STATS_INC(link.xmit);

    return ERR_OK;
}