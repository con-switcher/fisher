//
// Created by tsxuehu on 2019-04-07.
//

#ifndef LWIP_BIN_SOCKSCLIENT_H
#define LWIP_BIN_SOCKSCLIENT_H

/**
 * 和服务器 socks连接客户端
 * 1. 创建socks连接
 * 2. 发送数据，数据发送后回调通知发送完成
 * 3. 接受数据，通知外部
 */
class SocksClient {

private:
    // ip port
    // 写回调
    // 读回调
public:
    // 建立连接后，进行socks5协议认证
    void connect();

};


#endif //LWIP_BIN_SOCKSCLIENT_H
