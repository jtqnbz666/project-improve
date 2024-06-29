#ifndef BaseConn_H
#define BaseConn_H

#include "CommType.h"
#include <string>
#include "InfiNet.h"
#include <unordered_map>
#include "PduUtil.h"
#include "ThreadPool.h"
#include "BasePdu.h"

using namespace std;

enum {
    SOCKET_STATE_IDLE,
    SOCKET_STATE_LISTENING,
    SOCKET_STATE_CONNECTING,
    SOCKET_STATE_CONNECTED,
    SOCKET_STATE_CLOSING
};


#define READ_BUF_SIZE               2048

void BaseConnCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam);

class BaseConn {
public:
    BaseConn();
    ~BaseConn();
    int GetSocket() { return m_socket; }
    void SetSocket(int fd) { m_socket = fd; }
    void SetState(uint8_t state) { m_state = state; }
    void SetCallback(callbackFunc cbFunc) { m_cbFunc = cbFunc; }
    void SetCallbackData(void* cbData) { m_cbData = cbData; }
   


    uchar_t* GetReadBuffer() { return m_readBuf.GetBuffer(); }
    uchar_t* GetWriteBuffer() { return m_writeBuf.GetBuffer(); }



    void SetRemoteIP(string ip) { m_remoteIp = ip; }
    void SetRemotePort(uint16_t port) { m_remotePort = port; }
    void SetLocalIP(string ip) { m_remoteIp = ip; }
    void SetLocalPort(uint16_t port) { m_remotePort = port; }

    string GetLocalIP() { return m_localIp; }
    uint16_t GetLocalPort() { return m_localPort; }
    string GetRemoteIP() { return m_remoteIp; }
    uint16_t GetRemotePort() { return m_remotePort; }

public:
    int UDPListen(const char* server_ip, uint16_t port, callbackFunc cbFunc, void* cbData);
    void Accept();
    void UDPAccept();
    int Connect(const char* server_ip, uint16_t port, callbackFunc cbFunc, void* cbData);
    int UDPSend(void* buf, int len);
    int UDPRecv(void* buf, int len);

    void OnRead();
    void OnWrite();
    void OnClose();

    void SetTcpMode(bool isTcpMode);
    bool IsTcpMode() { return m_isTcpMode; }


    static unordered_map<int, BaseConn*> globalSocketMap;
    void OnTcpRead();
    void OnUdpRead();

private:
 

    void _SetReuseAddr(int fd);
    void _SetReusePort(int fd);
    void _SetNonblock(int fd);
    void _SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr);
    

    string          m_remoteIp;
    string          m_localIp;
    uint16_t        m_remotePort;
    uint16_t        m_localPort;
    

    callbackFunc    m_cbFunc;
    void*           m_cbData; 

    uint8_t         m_state;
    int             m_socket;
    bool            m_isTcpMode;

    InfiBuffer       m_readBuf;       //读写缓冲区分开
    InfiBuffer       m_writeBuf;
};

BaseConn*  FindConn(int fd);

#endif