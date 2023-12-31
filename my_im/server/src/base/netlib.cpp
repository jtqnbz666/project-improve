#include "netlib.h"
#include "BaseSocket.h"
#include "EventDispatch.h"
//netlib网络库的封装，实际上的底层实现在BaseSokect和EventDispatch中
//BaseSocket主要负责对fd的封装， EventDispatch负责事件循环和定时器
int netlib_init() {
    int ret = NETLIB_OK;
    return ret;
}

int netlib_destroy() {
    int ret = NETLIB_OK;
    return ret;
}

int netlib_listen(
    const char* server_ip,
    uint16_t    port,
    callback_t  callback,
    void*       callback_data) 
{
    CBaseSocket* pSocket = new CBaseSocket();
    if(!pSocket) {
        return NETLIB_INVALID_HANDLE;
    }
    int ret = pSocket->Listen(server_ip, port, callback, callback_data);
    if(ret == NETLIB_ERROR) {
        delete pSocket;
    }
    return ret;
}

net_handle_t netlib_connect(
    const char*     server_ip,
    uint16_t        port,
    callback_t      callback,
    void*           callback_data)
{
    CBaseSocket* pSocket = new CBaseSocket();
    if(!pSocket) return NETLIB_INVALID_HANDLE;
    net_handle_t handle = pSocket->Connect(server_ip, port, callback, callback_data);
    if(handle == NETLIB_INVALID_HANDLE) delete pSocket;
    return handle;
}

int netlib_send(net_handle_t handle, void* buf, int len) {
    CBaseSocket* pSocket = FindBaseSocket(handle);
    if(!pSocket) {
        return NETLIB_ERROR;
    }
    int ret = pSocket->Send(buf, len);
    pSocket->ReleaseRef();
    return ret;
}

int netlib_recv(net_handle_t handle, void* buf, int len) {
    CBaseSocket* pSocket = FindBaseSocket(handle);
    if(!pSocket) {
        return NETLIB_ERROR;
    }
    int ret = pSocket->Recv(buf, len);
    pSocket->ReleaseRef();
    return ret;
}

int netlib_close(net_handle_t handle) {
    CBaseSocket* pSocket = FindBaseSocket(handle);
    if(!pSocket) return NETLIB_ERROR;
    int ret = pSocket->Close();
    pSocket->ReleaseRef();
    return ret;
}

int netlib_option(net_handle_t handle, int opt, void* optval) {
    CBaseSocket* pSocket = FindBaseSocket(handle);
    if(!pSocket) {
        return NETLIB_ERROR;
    }
    if( (opt >= NETLIB_OPT_GET_REMOTE_IP) && !optval) return NETLIB_ERROR;
    switch(opt) {
    case NETLIB_OPT_SET_CALLBACK:
        pSocket->SetCallback((callback_t)optval);
        break;
    case NETLIB_OPT_SET_CALLBACK_DATA:
        pSocket->SetCallbackData(optval);
        break;
    case NETLIB_OPT_GET_REMOTE_IP:
        *(string*)optval = pSocket->GetRemoteIP();
        break;
    case NETLIB_OPT_GET_REMOTE_PORT:
        *(uint16_t*)optval = pSocket->GetRemotePort();
        break;
    case NETLIB_OPT_GET_LOCAL_IP:
        *(string*)optval = pSocket->GetLocalIP();
        break;
    case NETLIB_OPT_GET_LOCAL_PORT:
        *(uint16_t*)optval = pSocket->GetLocalPort();
        break;
    case NETLIB_OPT_SET_SEND_BUF_SIZE:
        pSocket->SetSendBufSize(*(uint32_t*)optval);
        break;
    case NETLIB_OPT_SET_RECV_BUF_SIZE:
        pSocket->SetRecvBufSize(*(uint32_t*)optval);
        break;
    }
    pSocket->ReleaseRef(); //引用减一；；
    //因为刚创建的时候就加一了， 设置好属性就减回去
    //imconn.cpp 3行 可以看到
    return NETLIB_OK;
}

int netlib_register_timer(callback_t callback, void* user_data, uint64_t interval) {
    CEventDispatch::Instance()->AddTimer(callback, user_data, interval);
    return 0;
}

int netlib_delete_timer(callback_t callback, void* user_data) {
    CEventDispatch::Instance()->RemoveTimer(callback, user_data);
    return 0;
}

int netlib_add_loop(callback_t callback, void* user_data) {
    CEventDispatch::Instance()->AddLoop(callback, user_data);
    return 0;
}

void netlib_eventloop(uint32_t wait_timeout) {
    CEventDispatch::Instance()->StartDispatch(wait_timeout);
}

void netlib_stop_event() {
    CEventDispatch::Instance()->StopDispatch();
}

bool netlib_is_running() {
    return CEventDispatch::Instance()->isRunning();
}