#ifndef CONNMANAGER_H
#define CONNMANAGER_H
#include "BaseConn.h"
#include "CommType.h"
class ConnManager {
public:
    static ConnManager& Instance();

    void AddConn(BaseConn* conn);
    void RemoveConn(int fd);
    shared_ptr<BaseConn> GetConn(int fd);

    void SingleForward(int fd, InfiPdu* message);
    void BroadcastMessage(InfiPdu* message);
    void BroadcastMessageExceptSender(int fd, InfiPdu* message);


private:
    ConnManager() {}
    ~ConnManager() {}
    ConnManager(const ConnManager&) = delete;
    ConnManager& operator=(const ConnManager&) = delete;

    unordered_map<int, shared_ptr<BaseConn>> connections;

};

#endif