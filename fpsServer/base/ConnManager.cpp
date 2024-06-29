#include "ConnManager.h"


// 获取单例实例
ConnManager& ConnManager::Instance() {
    static ConnManager instance;
    return instance;
}

// 添加连接
void ConnManager::AddConn(BaseConn* conn) {
    connections[conn->GetSocket()] = shared_ptr<BaseConn>(conn);
}

// 删除连接
void ConnManager::RemoveConn(int fd) {
    connections.erase(fd);
}

// 获取连接
shared_ptr<BaseConn> ConnManager::GetConn(int fd) {
    if (connections.find(fd) != connections.end()) {
        return connections[fd];
    }
    return nullptr;
}

// 广播消息
void ConnManager::BroadcastMessage(InfiPdu* message) {
    for (auto& pair : connections) {
        if(pair.second->GetState() == SOCKET_STATE_LISTENING) continue;
        pair.second->SendPdu(message);
    }
}

void ConnManager::BroadcastMessageExceptSender(int fd, InfiPdu* message) {
    for (auto& pair : connections) {
        if(pair.second->GetState()== SOCKET_STATE_LISTENING ||
        pair.second->GetSocket() == fd) continue;
        pair.second->SendPdu(message);
    }
}

// 发给指定对象
void ConnManager::SingleForward(int fd, InfiPdu* message) {
    if (connections.find(fd) != connections.end()) {
        connections[fd]->SendPdu(message);
    }
}