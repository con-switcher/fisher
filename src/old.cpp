#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sys_domain.h>
#include <sys/kern_control.h>
#include <net/if_utun.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <stdlib.h>

#include <string>
#include <iostream>

// Open specific utun device unit and return fd.
// If the unit number is already in use, return -1.
// Throw exceptions for all other errors.
// Return the iface name in name.
int tun_open(std::string &name, const int unit)
{
    struct sockaddr_ctl sockaddrCtl;
    struct ctl_info ctlInfo;

    memset(&ctlInfo, 0, sizeof(ctlInfo));

    strlcpy(ctlInfo.ctl_name, UTUN_CONTROL_NAME, sizeof(ctlInfo.ctl_name));

    // 创建socket https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/socket.2.html#//apple_ref/doc/man/2/socket
    int fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (fd < 0)
    {
        perror("socket");
        return -1;
    }

    // 写入控制信息 https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/ioctl.2.html#//apple_ref/doc/man/2/ioctl
    if (ioctl(fd, CTLIOCGINFO, &ctlInfo) == -1)
    {
        perror("CTLIOCGINFO");
        close(fd);
        return -1;
    }

    sockaddrCtl.sc_id = ctlInfo.ctl_id;
    sockaddrCtl.sc_len = sizeof(sockaddrCtl);
    sockaddrCtl.sc_family = AF_SYSTEM;
    sockaddrCtl.ss_sysaddr = AF_SYS_CONTROL;
    sockaddrCtl.sc_unit = unit;

    // https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/connect.2.html#//apple_ref/doc/man/2/connect
    // If the connect is successful, a utunX device will be created, where X
    // is our unit number - 1.
    if (connect(fd, (struct sockaddr *)&sockaddrCtl, sizeof(sockaddrCtl)) == -1)
    {
        perror("connect ");
        close(fd);
        return -1;
    }

    // 获取 https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/getsockopt.2.html
    // Get iface name of newly created utun dev.
    char utunname[20];
    socklen_t utunname_len = sizeof(utunname);
    if (getsockopt(fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, utunname, &utunname_len))
    {
        perror("getsockopt");
        close(fd);
        return -1;
    }
    name = utunname;

    return fd;
}

// Try to open an available utun device unit.
// Return the iface name in name.
int tun_open(std::string &name)
{
    for (int unit = 0; unit < 256; ++unit)
    {
        const int fd = tun_open(name, unit + 1);
        if (fd >= 0)
            return fd;
    }
    return -1;
}

void tun_close(int fd)
{
    close(fd);
}

int main(int argc, char **argv)
{
    std::string tun_name;
    int utunfd = tun_open(tun_name);
    std::cout << tun_name <<std::endl;
    if (utunfd == -1)
    {
        fprintf(stderr, "Unable to establish UTUN descriptor - aborting\n");
        exit(1);
    }

    fprintf(stderr, "Utun interface is up.. Configure IPv4 using \"ifconfig utun1 _ipA_ _ipB_\"\n");
    fprintf(stderr, "                       Configure IPv6 using \"ifconfig utun1 inet6 _ip6_\"\n");
    fprintf(stderr, "Then (e.g.) ping _ipB_ (IPv6 will automatically generate ND messages)\n");

    // PoC - Just dump the packets...
    for (;;)
    {
        unsigned char c[1500];
        int len;
        int i;

        len = read(utunfd, c, 1500);

        // First 4 bytes of read data are the AF: 2 for AF_INET, 1E for AF_INET6, etc..
        for (i = 4; i < len; i++)
        {
            printf("%02x ", c[i]);
            if ((i - 4) % 16 == 15)
                printf("\n");
        }
        printf("\n");
    }

    return (0);
}