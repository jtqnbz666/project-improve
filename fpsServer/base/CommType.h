#ifndef COMM_TYPE_H
#define COMM_TYPE_H


#include <sys/epoll.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>		// define int8_t ...
#include <signal.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <assert.h>
#include <iostream>



using namespace std;

typedef unsigned char   uchar_t;

typedef void (*callbackFunc)(void* callbackData, uint8_t msg, uint32_t handle, void* pParam);

enum {
    INFINET_MSG_CONNECT = 1,
    INFINET_MSG_CONFIRM,
    INFINET_MSG_READ,
    INFINET_MSG_WRITE,
    INFINET_MSG_CLOSE,
    INFINET_MSG_TIMER,
    INFINET_MSG_LOOP
};
#endif