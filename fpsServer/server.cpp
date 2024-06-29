#include "InfiNet.h"
//#include "MsgConn.h"
#include "ThreadPool.h"
#include "RouterFactory.h"
#include "RouterRegister.h"

// void msgServerFunc(void* callbackData, uint8_t msg, uint32_t fd, void* pParam) {
//     printf("成功触发我!!!\n");
//     MsgConn* newConn = new MsgConn();
//     newConn->InitMsgConn(fd);
// }

int main() {
    //registerAllRouters();
    ThreadPool::Instance()->Init(8);
    infiNet_udpListen("127.0.0.1", 1234, nullptr, nullptr);
    infiNet_startLoop();
}