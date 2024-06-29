// 优化点：把连接的封装和对连接的管理（增删等）分开

#include "BaseConn.h"
#include "EventDispatch.h"

typedef unordered_map<int, BaseConn*> SocketMap;
unordered_map<int, BaseConn*> BaseConn::globalSocketMap;
   
// 初始化静态成员变量
void BaseConnCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
    BaseConn* conn = FindConn(handle);
    printf("trigger BaseConnCallback!!!\n");
    if (conn->IsTcpMode()) {
        conn->OnTcpRead();
    } else {
        conn->OnUdpRead();
    }
}

void AddBaseConn(BaseConn* bConn) {
    BaseConn::globalSocketMap.insert(make_pair(bConn->GetSocket(), bConn));
}

void RemoveBaseConn(BaseConn* bConn) {
    BaseConn::globalSocketMap.erase(bConn->GetSocket());
}

BaseConn* FindConn(int fd) {
    BaseConn* bConn = NULL;
    SocketMap::iterator iter = BaseConn::globalSocketMap.find(fd);
    if(iter != BaseConn::globalSocketMap.end()) {
        bConn = iter->second;
    }
    return bConn;
}

BaseConn::BaseConn() {
    m_socket = -1;
    m_state = -1;
}

BaseConn::~BaseConn() {
}

int BaseConn::UDPListen(const char* serverIp, uint16_t port, callbackFunc cbFunc, void* cbData) {
    m_localIp = serverIp;
    m_localPort = port;
    m_cbFunc = cbFunc;
    m_cbData = cbData;

    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_socket == -1) {
            printf("socket failed, serverIp = %s, port = %u, listen_socket = %d",  serverIp, port, m_socket);
            return -1;
    } 
    _SetReuseAddr(m_socket);
    _SetReusePort(m_socket);
    _SetNonblock(m_socket);
    SetTcpMode(false);

    sockaddr_in serv_addr;
    _SetAddr(serverIp, port, &serv_addr);

    
    int ret = ::bind(m_socket, (sockaddr*)&serv_addr, sizeof(serv_addr));
    
    if(ret) {
            printf("bind failed, server_ip=%s, port=%u\n", serverIp, port);
            close(m_socket); // 就是  close(m_socket);
            return -1;
    }
    m_state = SOCKET_STATE_LISTENING;  
    printf("BaseConn::Listen on %s:%d, listenSocket:%d\n", serverIp, port, m_socket);
    AddBaseConn(this);
    EventDispatch::Instance()->AddEvent(m_socket, EPOLLIN | EPOLLET);
    return 0;
}

void BaseConn::Accept() {
    if (infiNet_tcpMode(m_socket)) {

    } else {
        UDPAccept();
    }
}

void BaseConn::UDPAccept() {
    int new_sd = -1;
    int ret = 0;
    int reuse = 1;
    char buf[16];
    struct sockaddr_in peer_addr;
    socklen_t cli_len = sizeof(peer_addr);
    ret = recvfrom(m_socket, buf, 16, 0, (struct sockaddr *)&peer_addr, &cli_len);
    if (ret < 0) {
        perror("recvfrom failed");
        return;
    }

    BaseConn* bConn = new BaseConn();
    char* ip = inet_ntoa(peer_addr.sin_addr);
    uint16_t port = ntohs(peer_addr.sin_port);
    if ((new_sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("child socket failed");
        exit(1);
    } 

    printf("AcceptNewSocket=%d parent:%d from %s:%d\n", new_sd, m_socket, ip, port);
   
    SetTcpMode(false);
    _SetReuseAddr(new_sd);
    _SetReusePort(new_sd);
    _SetNonblock(new_sd);

    sockaddr_in m_addr;
    _SetAddr(m_localIp.c_str(), m_localPort, &m_addr);
    ret = bind(new_sd, (sockaddr*)&m_addr, sizeof(m_addr));
    if (ret){
        perror("chid bind failed");
        exit(1);
    } 

    peer_addr.sin_family = PF_INET;
    if (connect(new_sd, (struct sockaddr *) &peer_addr, sizeof(struct sockaddr)) == -1) {
        perror("chid connect failed");
        exit(1);
    } 

    bConn->SetSocket(new_sd);
    bConn->SetCallback(m_cbFunc); 
    bConn->SetCallbackData(m_cbData);
    bConn->SetState(SOCKET_STATE_CONNECTED);
    bConn->SetRemoteIP(ip);
    bConn->SetRemotePort(port);

    AddBaseConn(bConn);
    EventDispatch::Instance()->AddEvent(new_sd, EPOLLIN);
    m_cbFunc(m_cbData, INFINET_MSG_CONNECT, new_sd, NULL);
}

void BaseConn::_SetReuseAddr(int fd) {
    int reuse = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if(ret) {
        perror("_SetReuseAddr failed");
        }
}

void BaseConn::_SetReusePort(int fd) {
    int reuse = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
    if (ret) {
        perror("_SetReusePort failed ");
    }
}

void BaseConn::_SetNonblock(int fd) {
    int ret = fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL));
    if(ret == -1) {
        perror("_SetNonblock failed ");
    }
}

void BaseConn::_SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr) {
    memset(pAddr, 0, sizeof(sockaddr_in));
    pAddr->sin_family = AF_INET;
    pAddr->sin_port = htons(port);
    pAddr->sin_addr.s_addr = inet_addr(ip);
}


void BaseConn::OnRead() {
    if(m_state == SOCKET_STATE_LISTENING) {
        Accept();
    } else {
       m_cbFunc(nullptr, INFINET_MSG_READ, m_socket, nullptr);
    }
}

void BaseConn::OnTcpRead() {
}

void BaseConn::OnUdpRead() {
    uint32_t freeBuffSize = m_readBuf.GetAllocSize() - m_readBuf.GetWriteOffset();
    if(freeBuffSize < READ_BUF_SIZE) {
        printf("Extend ReadBuf\n");
        m_readBuf.Extend(READ_BUF_SIZE);
    }

    struct sockaddr_in peer_addr;
    int ret = infiNet_udpRecv(m_socket, m_readBuf.GetBuffer() + m_readBuf.GetWriteOffset(), READ_BUF_SIZE, &peer_addr);
    if(ret <= 0) {
        perror("infiNet_udpRecv failed");
    }
    m_readBuf.IncWriteOffset(ret); //记录大小

    InfiPdu* pdu = nullptr;
    pdu = InfiPdu::ReadPdu(m_readBuf.GetBuffer(), m_readBuf.GetWriteOffset());
    if (!pdu) {
        printf("UnPackPdu failed\n");
    }
    pdu->SetBaseConn(this);

    // 把pdu封装成task放到线程池中。
    CTask* task = new CTask();
    task->SetPdu(pdu);
    ThreadPool::Instance()->AddTask(task);
    m_readBuf.Read(nullptr, pdu->GetBufferLength());
}

void BaseConn::OnWrite() {
}

void BaseConn::OnClose() {
}

void BaseConn::SetTcpMode(bool isTcpMode) {
    m_isTcpMode = isTcpMode;
}
