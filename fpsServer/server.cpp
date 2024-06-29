#include "InfiNet.h"
//#include "MsgConn.h"
#include "ThreadPool.h"
#include "RouterFactory.h"
#include "RouterRegister.h"
#include "TimerNode.h"

// void msgServerFunc(void* callbackData, uint8_t msg, uint32_t fd, void* pParam) {
//     printf("成功触发我!!!\n");
//     MsgConn* newConn = new MsgConn();
//     newConn->InitMsgConn(fd);
// }
void OnHeartBeatTimer() {
    printf("OnHeartBeatTimer\n");
    infiNet_registerTimer(1000, OnHeartBeatTimer);
}

int main() {
    //registerAllRouters();
    ThreadPool::Instance()->Init(8);
    infiNet_udpListen("127.0.0.1", 1234, nullptr, nullptr);
    // 添加定时任务
    infiNet_registerTimer(1000, OnHeartBeatTimer);
    infiNet_startLoop();
}