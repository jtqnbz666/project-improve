#ifndef INFINET_H
#define INFINET_H
#include "CommType.h"
#include "BaseConn.h"
#include "BasePdu.h"


enum {
    INFINET_OK = 0,
    INFINET_ERROR = 1
};

int infiNet_startLoop();

int infiNet_destroy();

int infiNet_udpListen(const char* serverIP, uint16_t port, callbackFunc cbFunc, void* cbData);

int infiNet_connect(const char* serverIp, uint16_t port, callbackFunc cbFunc, void* cbData);

int infiNet_send(int fd, void* buf, int len);

int infiNet_udpRecv(int fd, void* buf, int len, sockaddr_in* peer_addr);

int infiNet_close(int fd);

bool infiNet_tcpMode(int fd);

#endif