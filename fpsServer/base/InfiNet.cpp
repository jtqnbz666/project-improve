#include "InfiNet.h"
#include "EventDispatch.h"
int infiNet_startLoop() {
    EventDispatch::Instance()->StartDispatch(0);
}

int infiNet_registerTimer(time_t msec, Callback func) {
    EventDispatch::Instance()->GetTimerManager()->AddTimer(msec, func);
}

int infiNet_destroy() {
    return 0; 
}

int infiNet_udpListen(const char* serverIP, uint16_t port, callbackFunc cbFunc,  void* cbData) {
    BaseConn* bConn = new BaseConn();
    if(!bConn) {
        return -1;
    }

    int ret = bConn->UDPListen(serverIP, port, cbFunc, cbData);
    if(ret == -1) {
        delete bConn;
    }
    return ret;
}

int infiNet_connect(const char* serverIp, uint16_t port, callbackFunc cbFunc, void* cbData) {

}

int infiNet_send(int fd, void* buf, int len) {

}

int infiNet_udpRecv(int fd, void* buf, int len, sockaddr_in* peer_addr) {
    socklen_t cli_len = sizeof(peer_addr);
    int ret = recvfrom(fd, (char*)buf, len, 0, (struct sockaddr *)peer_addr, &cli_len);
    for (int i = 0; i < ret; ++i) {
        printf("%02X ", ((unsigned char*)buf)[i]);
    }

    return ret;
}

int infiNet_udpSend(int fd, void* buf, int len) {
    int ret = sendto(fd, buf, len, 0, nullptr, 0);
    printf("fd:%d, infiNet_udpWrite success!\n", fd);
    return ret;

}

int infiNet_close(int fd) {

}

bool infiNet_tcpMode(int fd) {
    int type;
    socklen_t length = sizeof(type);
    if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &type, &length) == -1) {
        perror("getsockopt");
        exit(1);
    }

    switch (type) {
        case SOCK_STREAM:
            return true;
        case SOCK_DGRAM:
            return false;
        default:
            perror("error mode");
            exit(1);
    }
}