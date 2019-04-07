
#include <stdlib.h>

#include <iostream>
#include <cstring>

#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/err.h"
#include "lwip/stats.h"
#include "lwip/sys.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/ip.h"
#include "lwip/tcp.h"




err_t lwip_netif_init_func(struct netif *netif);

err_t lwip_netif_data_out(struct netif *netif, struct pbuf *p, ip_addr_t *ipaddr);

err_t lwip_listener_accept_func (void *arg, struct tcp_pcb *newpcb, err_t err);


int main(int argc, char **argv) {

    char *ip_str = "192.168.10.1";

    char *mtu_str = "1500";

    lwip_init();

    // 初始化网卡， 设置网卡输入输出
    ip_addr_t ip, netmask, gw;
    struct netif netif;
    memset(&netif, 0, sizeof(netif));

    ipaddr_aton(ip_str, &ip);

    ip_addr_set_zero(&netmask);
    ip_addr_set_zero(&gw);
    netif_add(&netif, &ip, &netmask, &gw, NULL, lwip_netif_init_func, ip_input);
    netif.mtu = atoi(mtu_str);

    netif_set_default(&netif);
    netif_set_up(&netif);
    netif_set_link_up(&netif);

    // 初始化 tcp， 设置tcp层的监听、连接 读写函数
    struct tcp_pcb *listener;

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!pcb) {

    }
  /*  if (tcp_bind_to_netif(pcb, "ho0") != ERR_OK) {

    }
*/
    tcp_bind_netif(pcb, &netif);

    if (!(listener = tcp_listen(pcb))) {

    }

    tcp_accept(listener, lwip_listener_accept_func);

    std::cout << "hihi" << std::endl;

    // 启动libuv

    return (0);
}

// utun

// lwip netif

err_t lwip_netif_init_func(struct netif *netif) {
    netif->name[0] = 'u';
    netif->name[1] = 'n';
    netif->output = lwip_netif_data_out;
    return ERR_OK;
}

// lwip网卡数据输出
err_t lwip_netif_data_out(struct netif *netif, struct pbuf *p, ip_addr_t *ipaddr) {
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

// lwip tcp
err_t lwip_listener_accept_func (void *arg, struct tcp_pcb *newpcb, err_t err)
{
    ASSERT(err == ERR_OK)

    // allocate client structure
    struct tcp_client *client = (struct tcp_client *)malloc(sizeof(*client));
    if (!client) {
        BLog(BLOG_ERROR, "listener accept: malloc failed");
        goto fail0;
    }
    client->socks_username = NULL;

    SYNC_DECL
    SYNC_FROMHERE

    // read addresses
    client->local_addr = baddr_from_lwip(&newpcb->local_ip, newpcb->local_port);
    client->remote_addr = baddr_from_lwip(&newpcb->remote_ip, newpcb->remote_port);

    // get destination address
    BAddr addr = client->local_addr;
#ifdef OVERRIDE_DEST_ADDR
    ASSERT_FORCE(BAddr_Parse2(&addr, OVERRIDE_DEST_ADDR, NULL, 0, 1))
#endif

    // add source address to username if requested
    if (options.username && options.append_source_to_username) {
        char addr_str[BADDR_MAX_PRINT_LEN];
        BAddr_Print(&client->remote_addr, addr_str);
        client->socks_username = concat_strings(3, options.username, "@", addr_str);
        if (!client->socks_username) {
            goto fail1;
        }
        socks_auth_info[1].password.username = client->socks_username;
        socks_auth_info[1].password.username_len = strlen(client->socks_username);
    }

    // init SOCKS
    if (!BSocksClient_Init(&client->socks_client, socks_server_addr, socks_auth_info, socks_num_auth_info,
                           addr, (BSocksClient_handler)client_socks_handler, client, &ss)) {
        BLog(BLOG_ERROR, "listener accept: BSocksClient_Init failed");
        goto fail1;
    }

    // init aborted and dead_aborted
    client->aborted = 0;
    DEAD_INIT(client->dead_aborted);

    // add to linked list
    LinkedList1_Append(&tcp_clients, &client->list_node);

    // increment counter
    ASSERT(num_clients >= 0)
    num_clients++;

    // set pcb
    client->pcb = newpcb;

    // set client not closed
    client->client_closed = 0;

    // setup handler argument
    tcp_arg(client->pcb, client);

    // setup handlers
    tcp_err(client->pcb, client_err_func);
    tcp_recv(client->pcb, client_recv_func);

    // setup buffer
    client->buf_used = 0;

    // set SOCKS not up, not closed
    client->socks_up = 0;
    client->socks_closed = 0;

    client_log(client, BLOG_INFO, "accepted");

    DEAD_ENTER(client->dead_aborted)
    SYNC_COMMIT
    DEAD_LEAVE2(client->dead_aborted)

    // Return ERR_ABRT if and only if tcp_abort was called from this callback.
    return (DEAD_KILLED > 0) ? ERR_ABRT : ERR_OK;

    fail1:
    SYNC_BREAK
            free(client->socks_username);
    free(client);
    fail0:
    return ERR_MEM;
}




